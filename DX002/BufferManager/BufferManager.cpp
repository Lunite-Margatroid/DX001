#include "pch.h"
#include "BufferManager.h"
#include "Logger\FileLogger.h"
#include "InitBuffer.h"
#include "ConstantLightBuffer.h"
#include "ColoredCubeBuffer.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
#include "P3_N3_T2_Cube.h"
#include "P2_N2_Quad.h"

namespace yoi
{
	BufferManager::BufferManager(ID3D11Device* pDevice)
		:m_pDevice(pDevice)
	{
		const float side = 1.0f;
		struct Vertex
		{
			struct
			{
				float x, y, z;
			}Position;
			struct
			{
				float u, v;
			}TexCoord;
		};
		Vertex vertexData[] =
		{
			{1.0f, -1.0f, -1.0f,	0.0f, 0.0f},
			{1.0f, -1.0f, 1.0f,		1.0f, 0.0f},
			{1.0f, 1.0f, 1.0f,		1.0f, 1.0f},
			{1.0f, 1.0f, -1.0f,		0.0f, 1.0f},

			{-1.0f, -1.0f, -1.0f,	0.0f, 0.0f},
			{-1.0f, 1.0f, -1.0f,	1.0f, 0.0f},
			{-1.0f, 1.0f, 1.0f,		1.0f, 1.0f},
			{-1.0f, -1.0f, 1.0f,	0.0f, 1.0f},

			{ -1.0f,1.0f, -1.0f,	0.0f, 0.0f},
			{ 1.0f, 1.0f,-1.0f,		1.0f, 0.0f},
			{ 1.0f, 1.0f,1.0f,		1.0f, 1.0f},
			{ -1.0f,1.0f, 1.0f,		0.0f, 1.0f},

			{ -1.0f,-1.0f, -1.0f,	0.0f, 0.0f},
			{ -1.0f,-1.0f, 1.0f,	1.0f, 0.0f},
			{ 1.0f, -1.0f,1.0f,		1.0f, 1.0f},
			{ 1.0f, -1.0f,-1.0f,	0.0f, 1.0f},

			{ -1.f, -1.f,1.0f,		0.0f, 0.0f},
			{ -1.f, 1.f, 1.0f,		1.0f, 0.0f},
			{ 1.f, 1.f,  1.0f,		1.0f, 1.0f},
			{ 1.f, -1.f, 1.0f,		0.0f, 1.0f},

			{ -1.0f, -1.0f,-1.0f,	0.0f, 0.0f},
			{ 1.0f, -1.0f, -1.0f,	1.0f, 0.0f},
			{ 1.0f, 1.0f, -1.0f,	1.0f, 1.0f},
			{ -1.0f, 1.0f, -1.0f,	0.0f, 1.0f},
		};


		// 定义数据
		unsigned int indexData[36] = { 0,1,2,2,3,0 };
		for (int i = 6; i < 36; i++)
		{
			indexData[i] = indexData[i - 6] + 4;
		}

		// create vertex buffer
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(vertexData);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = vertexData;

		m_BufferMap[Buffer::Vertex_Textured_Cube] = AddBuffer(&bufferDesc, &subData);

		// create index buffer
		bufferDesc.ByteWidth = sizeof(indexData);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = sizeof(unsigned int);

		subData.pSysMem = indexData;
		m_BufferMap[Buffer::Index_Textured_Cube] = AddBuffer(&bufferDesc, &subData);


		// create constant buffer
		// 1. for material shininess
		float shininess = 64.0f;
		bufferDesc.ByteWidth = sizeof(float) * 4;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = 0u;
		subData.pSysMem = &shininess;
		AddBuffer(Buffer::Constant_Material_Shininess, &bufferDesc, &subData);

		// 2. for transform matrix
		BYTE constantData[sizeof(float) * 16 * 5 + sizeof(float) * 4];
		memset(constantData, 0, sizeof(float) * 16 * 5);

		bufferDesc.ByteWidth = sizeof(float) * 16 * 5 + sizeof(float) * 4;	// 5 * mat4x4
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = 0u;
		subData.pSysMem = constantData;
		m_BufferMap[Buffer::Constant_Matrix] = AddBuffer(&bufferDesc, &subData);

		

		ColoredCubeBuffer(this);
		ConstantLightBuffer(this);
		InitBuffer<Buffer::P3_N3_T2_Cube>(this);
		InitBuffer<Buffer::Vertex_P2_N2_Quad>(this);
	}
	BufferManager::~BufferManager()
	{
		for (ID3D11Buffer* pBuffer : m_Buffers)
		{
			pBuffer->Release();
		}
	}
	ID3D11Buffer* BufferManager::AddBuffer(const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData)
	{
		GFX_EXCEPT_SUPPORT();
		m_Buffers.push_back(nullptr);
		GFX_THROW_INFO(m_pDevice->CreateBuffer(pDescript, initData, &(m_Buffers.back())));
		return m_Buffers.back();
	}
	ID3D11Buffer* BufferManager::AddBuffer(Buffer buffer, const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData)
	{
		if(m_BufferMap.find(buffer) == m_BufferMap.end())
			return m_BufferMap[buffer] = AddBuffer(pDescript, initData);
		else
		{
			FileLogger::Warn("Same buffer is loaded");
			return m_BufferMap[buffer];
		}
	}
	ID3D11Buffer* BufferManager::GetBuffer(Buffer buffer)
	{
		if (m_BufferMap.find(buffer) == m_BufferMap.end())
		{// The buffer is not loaded.
			FileLogger::Error("The buffer is not loaded.");
			return nullptr;
		}
		return m_BufferMap[buffer];
	}
	ID3D11Buffer* BufferManager::SetBufferData(Buffer buffer, unsigned int size, unsigned int offset, void* src)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		ID3D11Buffer* buf = GetBuffer(buffer);
		if (!buf)
			return buf;
		D3D11_MAPPED_SUBRESOURCE dataMap;
		GFX_THROW_INFO(pContext->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataMap));
		memcpy(((BYTE*)dataMap.pData ) + offset, src, size);
		GFX_THROW_INFO_ONLY(pContext->Unmap(buf, 0));

		return buf;
	}
}