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
		m_NormalBuffer(width, height, DXGI_FORMAT_R32G32B32_FLOAT, 0u),
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

		/**************** load compute shader ****************/
		m_pConstraintShader = std::make_unique<CShader>(pDevice, "./shader-bin/ClothSimConstraintCS.cso");
		m_pUpdateShader = std::make_unique<CShader>(pDevice, "./shader-bin/ClothSimUpdateCS.cso");
		m_pGenNormal = std::make_unique<CShader>(pDevice, "./shader-bin/ClothSimGenNormal.cso");

		/******************* init const buffer **********************/
		ClothSimSettings settings = {};
		bd.ByteWidth = sizeof(ClothSimSettings);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = 0;

		sd.pSysMem = &settings;
		sd.SysMemPitch = 0u;
		m_cbufferSettings.Init(pDevice, &bd, &sd);

		/**************** create mesh ***********************/
		/*Mesh mesh(
			m_VertexBuffer.GetBuffer(),
			m_IndexBuffer.GetBuffer(),

		);*/
	}
	QuadCloth::~QuadCloth()
	{
	}
	void QuadCloth::Update(float deltaTime)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();

		// calc thread count
		int xCount = m_Width / 16;
		xCount = m_Width % 16 ? xCount : xCount + 1;
		int yCount = m_Height / 16;
		yCount = m_Height % 16 ? yCount : yCount + 1;
		// bind unorder access view
		/*
		RWTexture2D<float3> velRecord : register(u0);
		RWTexture2D<float3> preGrid : register(u1);
		RWTexture2D<float3> curGrid : register(u2);
		RWTexture2D<float3> normalGrid : register(u3);
		*/
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(0, 1, m_pVelRecordView.GetAddressOf(), nullptr));
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(1, 1, m_pPreGridView.GetAddressOf(), nullptr));
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(2, 1, m_pCurGridView.GetAddressOf(), nullptr));
		m_NormalBuffer.BindCS(pContext, 3);

		/********************* Update quad cloth ***************/
		// set constant buffer
		ClothSimSettings settings;
		settings.deltaTime = m_ConstDeltaTime;
		settings.nIteration = 20u;
		settings.alpha = m_Alpha;
		settings.m = m_Mass;
		settings.height = m_CellHeight;
		settings.width = m_CellWidth;
		settings.nHeight = m_Height;
		settings.nWidth = m_Width;
		settings.G = m_G;
		settings.tAlpha = m_Alpha / (m_ConstDeltaTime * m_ConstDeltaTime);
		settings.tBeta = 0.5f;
		m_cbufferSettings.SetBufferData(pContext, &settings, sizeof(ClothSimSettings), 0);

		// bind constant buffer
		ID3D11Buffer* constBuffer = m_cbufferSettings.GetBuffer();
		pContext->CSSetConstantBuffers(0, 1, &constBuffer);

		// bind compute shader
		m_pUpdateShader->Bind(pContext);
		// dispatch
		GFX_THROW_INFO_ONLY(pContext->Dispatch(xCount, yCount, 1));

		/******************* Constaint the vertex ********************/ 

		// bind compute shader
		m_pConstraintShader->Bind(pContext);

		// ----------- left top vertex ---------------
		settings.constraintU = 0u;
		settings.constraintV = 0u;
		settings.constraintX = 0.0f;
		settings.constraintY = 0.0f;
		settings.constraintZ = 0.0f;

		// set const buffer
		m_cbufferSettings.SetBufferData(pContext, &settings, sizeof(ClothSimSettings), 0);

		// dispatch
		GFX_THROW_INFO_ONLY(pContext->Dispatch(1, 1, 1));

		// right top vertex
		settings.constraintU = m_Width - 1;
		settings.constraintV = m_Height - 1;
		settings.constraintX = m_CellWidth * settings.constraintU;
		settings.constraintY = 0.0f;
		settings.constraintZ = 0.0f;

		// set const buffer
		m_cbufferSettings.SetBufferData(pContext, &settings, sizeof(ClothSimSettings), 0);

		// dispatch
		GFX_THROW_INFO_ONLY(pContext->Dispatch(1, 1, 1));

		/*********************** Generate the normal **************************/
		// bind compute shader
		m_pGenNormal->Bind(pContext);
		// Dispatch
		pContext->Dispatch(xCount, yCount, 1);

		m_NormalBuffer.UnbindCS(pContext, 3);

	}
}