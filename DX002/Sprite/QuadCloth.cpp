#include "pch.h"
#include "QuadCloth.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	QuadCloth::QuadCloth(Shader* shader, Material* material, const QuadClothDesc* desc)
		:QuadCloth(shader, material, desc->Width, desc->Height, desc->CellWidth, desc->CellHeight,
			desc->Alpha, desc->Beta, desc->ConstDeltaTime, desc->Mass, desc->G)
	{}

	QuadCloth::QuadCloth(
		Shader* shader, Material* material,
		unsigned int width, unsigned int height,
		float cellWidth, float cellHeight,
		float alpha, float beta,
		float constDeltaTime, float mass, float G)
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
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();


		struct Position { float x, y, z, w; };
		struct TexCoord { float u, v; };
		struct Normal { float x, y, z, w; };

		size_t sizeVertexBuffer[3] = { vertexCount * sizeof(Position) ,vertexCount * sizeof(TexCoord) , vertexCount * sizeof(Normal) };
		size_t sizeIndexBuffer = indexCount * sizeof(unsigned int);

		std::unique_ptr<BYTE[]> vertexBuffer(new BYTE[sizeVertexBuffer[0] + sizeVertexBuffer[1] + sizeVertexBuffer[2]]);
		std::unique_ptr<unsigned int[]> indexBuffer(new unsigned[indexCount]);

		Position* positionPtr = reinterpret_cast<Position*>(vertexBuffer.get());
		TexCoord* texCoordPtr = reinterpret_cast<TexCoord*>(positionPtr + vertexCount);
		Normal* normalPtr = reinterpret_cast<Normal*>(texCoordPtr + vertexCount);

		unsigned int counter = 0;
		/****************** init vertex buffer ********************/
		float deltaU = 1.0f / (m_Width - 1);
		float deltaV = 1.0f / (m_Height - 1);
		for (int i = 0; i < m_Height; i++)
			for (int j = 0; j < m_Width; j++)
			{
				Position& position = *(positionPtr + counter);
				TexCoord& texCoord = *(texCoordPtr + counter);
				counter++;

				position.x = j * m_CellWidth;
				position.y = -i * m_CellHeight;
				position.z = 0.f;
				position.w = 1.0f;

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
		// position
		bd.ByteWidth = sizeVertexBuffer[0] + sizeVertexBuffer[1];
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = 0u;

		sd.pSysMem = positionPtr;
		sd.SysMemPitch = sizeof(Position) * m_Width;

		m_pPositionBuffer = std::make_unique<DVertexBuffer>(
			VertexBuffer(Buffer(pDevice, &bd, &sd), 0, vertexCount), m_Width, m_Height, 4u
		);
		m_pPositionBuffer->SetLayoutType(1);
		m_pPositionBuffer->SetLayout<VertexBuffer::Distrib<float, 4>>();


		// vertexBuffer normal vector
		bd.ByteWidth = sizeVertexBuffer[2];
		sd.pSysMem = normalPtr;
		sd.SysMemPitch = sizeof(Normal) * m_Width;
		m_pNormalBuffer = std::make_unique<DVertexBuffer>(
			VertexBuffer(Buffer(pDevice, &bd, &sd), 0, vertexCount), m_Height, m_Width, 4u
		);
		m_pNormalBuffer->SetLayoutType(1);
		m_pNormalBuffer->SetLayout<VertexBuffer::Distrib<float, 4>>(); 

		// vertex buffer texcoord buffer
		bd.ByteWidth = sizeVertexBuffer[1];
		sd.pSysMem = texCoordPtr;
		sd.SysMemPitch = 0u;
		m_pTexCoordBuffer = std::make_unique<VertexBuffer>(
			Buffer(pDevice, &bd, &sd), 0, vertexCount
		);
		m_pTexCoordBuffer->SetLayoutType(1);
		m_pTexCoordBuffer->SetLayout<VertexBuffer::Distrib<float, 2>>();



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
		m_IndexBuffer.SetIndexCount(indexCount);
		m_IndexBuffer.SetOffset(0u);

		/*************** create temp buffer ****************/
		
		m_pPreGridBuffer = std::make_unique<UATexture>(m_Width, m_Height, static_cast<void*>(positionPtr), static_cast<D3D11_CPU_ACCESS_FLAG>(0u), DXGI_FORMAT_R32G32B32A32_FLOAT);
		m_pVelRecordBuffer = std::make_unique<UATexture>(m_Width, m_Height, nullptr, static_cast<D3D11_CPU_ACCESS_FLAG>(0u), DXGI_FORMAT_R32G32B32A32_FLOAT);



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

		Mesh mesh(
			std::vector<VertexBuffer*>(
				{static_cast<VertexBuffer*>(m_pPositionBuffer.get()), 
				static_cast<VertexBuffer*>(m_pNormalBuffer.get()),
				static_cast<VertexBuffer*>(m_pTexCoordBuffer.get())
				}),
			&m_IndexBuffer, 
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 
			shader, 
			material);

		AddMesh(std::move(mesh));
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
		xCount = m_Width % 16 ? xCount + 1: xCount;
		int yCount = m_Height / 16;
		yCount = m_Height % 16 ? yCount + 1: yCount;
		// bind unorder access view
		/*
		RWTexture2D<float3> velRecord : register(u0);
		RWTexture2D<float3> preGrid : register(u1);
		RWTexture2D<float3> curGrid : register(u2);
		RWTexture2D<float3> normalGrid : register(u3);
		*/
		m_pPreGridBuffer  ->BindCS(pContext, 1u);
		m_pVelRecordBuffer->BindCS(pContext, 0u);

		m_pPositionBuffer->UnbindVertexBuffer(pContext, 0);
		m_pNormalBuffer->UnbindVertexBuffer(pContext, 1);

		m_pPositionBuffer ->BindCS(pContext, 2);
		m_pNormalBuffer   ->BindCS(pContext, 3);

		// GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(2, 1, m_pCurGridView.GetAddressOf(), nullptr));

		

		/********************* Update quad cloth ***************/
		// set constant buffer
		ClothSimSettings settings;
		settings.deltaTime = m_ConstDeltaTime;
		settings.nIteration = 40u;
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
		settings.constraintV = 0u;
		settings.constraintX = m_CellWidth * settings.constraintU;
		settings.constraintY = 0.0f;
		settings.constraintZ = 0.1f;

		// set const buffer
		m_cbufferSettings.SetBufferData(pContext, &settings, sizeof(ClothSimSettings), 0);

		// dispatch
		GFX_THROW_INFO_ONLY(pContext->Dispatch(1, 1, 1));

		/*********************** Generate the normal **************************/
		// copy curGrid -> preGrid is also occur here
		// bind compute shader
		m_pGenNormal->Bind(pContext);
		// Dispatch
		pContext->Dispatch(xCount, yCount, 1);


		/********************** unbind the view on vertex buffer***************/
		m_pPositionBuffer->UnbindCS(pContext, 2);
		m_pNormalBuffer->UnbindCS(pContext, 3);
	}
}