#include "pch.h"
#include "QuadCloth.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	QuadCloth::QuadCloth(
		unsigned int width, unsigned int height, 
		float cellWidth , float cellHeight, 
		float alpha ,float beta, 
		float constDeltaTime , float mass, float G)
		:
		m_Width(width), m_Height(height),
		m_CellWidth(cellWidth), m_CellHeight(cellHeight),
		m_Alpha(alpha), m_Beta(beta),
		m_ConstDeltaTime(constDeltaTime), m_Mass(mass), m_G(G)
	{
		D3D11_BUFFER_DESC bd = {};
		D3D11_SUBRESOURCE_DATA sd = {};

		unsigned int vertexCount = width * height;
		unsigned int indexCount = (height - 1) * width * 2 + height - 1;

		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice =  Graphics::GetInstance().GetDevice();
		

		struct Position { float x, y, z; };
		struct TexCoord { float u, v; };

		size_t sizeVertexBuffer = vertexCount * (sizeof(Position) + sizeof(TexCoord));
		size_t sizeIndexBuffer = indexCount * sizeof(unsigned int);

		std::unique_ptr<BYTE[]> vertexBuffer(new BYTE[sizeVertexBuffer]);
		std::unique_ptr<unsigned int[]> indexBuffer ( new unsigned[indexCount]);

		Position* positionPtr = reinterpret_cast<Position*>(vertexBuffer.get());
		TexCoord* texCoordPtr = reinterpret_cast<TexCoord*>(positionPtr + vertexCount);

		unsigned int counter = 0;
		/****************** init vertex buffer ********************/
		float deltaU = 1.0f / (m_Width - 1);
		float deltaV = 1.0f / (m_Height - 1);
		for(int i =0 ;i < m_Height;i++)
			for (int j = 0; j < m_Width; j++)
			{
				Position& position = *(positionPtr + counter);
				TexCoord& texCoord = *(texCoordPtr + counter);
				counter++;

				position.x = j * m_CellWidth;
				position.y = -i * m_CellHeight;
				position.z = 0.f;

				texCoord.u = j * deltaU;
				texCoord.v = i * deltaV;
			}

		/**************** init index buffer *********************/
		counter = 0;
		for (int i = 0; i < m_Height - 1; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				indexBuffer[counter++] = j + i * m_Width;
				indexBuffer[counter++] = j + (i + 1) * m_Width;
			}
			indexBuffer[counter++] = 0xffffffff;
		}

		/************** create d3d buffer ************/
		// vertex buffer
		bd.ByteWidth = sizeVertexBuffer;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = 0u;

		sd.pSysMem = vertexBuffer.get();
		sd.SysMemPitch = sizeof(Position) * m_Width;

		m_VertexBuffer.Init(pDevice, &bd, &sd);
		m_VertexBuffer.PushAttrib<float>(3);
		m_VertexBuffer.PushAttrib<float>(2);
		m_VertexBuffer.SetLayoutType(1);

		// index buffer
		bd.ByteWidth = sizeIndexBuffer;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = 0u;

		sd.pSysMem = indexBuffer.get();
		sd.SysMemPitch = 0u;

		m_IndexBuffer.Init(pDevice, &bd, &sd);

		/*************** create temp buffer ****************/
		// position temp buffer
		bd.ByteWidth = sizeof(Position) * vertexCount;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = 0u;

		sd.pSysMem = vertexBuffer.get();
		sd.SysMemPitch = sizeof(Position) * m_Width;
		GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &m_pPreGridBuffer));

		// velocity record buffer
		memset(vertexBuffer.get(), 0, sizeof(Position)* vertexCount);
		BYTE* velocityBuffer = vertexBuffer.get();
		bd.ByteWidth = sizeof(float) * 3 * vertexCount;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = 0u;

		sd.pSysMem = velocityBuffer;
		sd.SysMemPitch = 0u;

		GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &m_pVelRecordBuffer));

		/******************** Resource Veiw ***********************/
		// cur grid unorder access view
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavd = {};
		uavd.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavd.Texture2D.MipSlice = 0u;

		GFX_THROW_INFO(pDevice->CreateUnorderedAccessView(m_VertexBuffer.GetBuffer(), &uavd, &m_pCurGridView));

		// pre grid unorder access view
		GFX_THROW_INFO(pDevice->CreateUnorderedAccessView(m_pPreGridBuffer.Get(), &uavd, &m_pPreGridView));

		// velocity record unorder access view
		GFX_THROW_INFO(pDevice->CreateUnorderedAccessView(m_pVelRecordBuffer.Get(), &uavd, &m_pVelRecordView));
	}
	QuadCloth::~QuadCloth()
	{
	}
}