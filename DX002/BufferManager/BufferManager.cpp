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
#include "Vertex_P3_N3_T2_Quad.h"


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

		VertexBuffer* vb = dynamic_cast<VertexBuffer*>(m_BufferMap[Buffers::Vertex_Textured_Cube] = AddVertexBuffer(Buffer(m_pDevice, &bufferDesc, &subData), 0, 24));
		vb->SetLayout<VertexBuffer::Distrib<float, 3>, VertexBuffer::Distrib<float, 2>>();


		// create index buffer
		bufferDesc.ByteWidth = sizeof(indexData);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = sizeof(unsigned int);

		subData.pSysMem = indexData;
		m_BufferMap[Buffers::Index_Textured_Cube] = AddIndexBuffer(Buffer(m_pDevice, &bufferDesc, &subData), 36, 0);


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
		AddConstBuffer(Buffers::Constant_Material_Shininess,m_pDevice, sizeof(float) * 4, &subData);

		// 2. for transform matrix
		BYTE constantData[sizeof(float) * 16 * 5 + sizeof(float) * 4];
		memset(constantData, 0, sizeof(float) * 16 * 5);
		subData.pSysMem = constantData;
		m_BufferMap[Buffers::Constant_Matrix] = AddConstBuffer(m_pDevice, sizeof(float) * 16 * 5 + sizeof(float) * 4, &subData);

		// create constant buffer for water wave upadating
		float waterwaveUpdateSetting[4] = {0.0f, 0.0f, 0.0f, 0.01f};
		subData.pSysMem = waterwaveUpdateSetting;
		m_BufferMap[Buffers::Constant_Water_Wave_Update] = AddConstBuffer(m_pDevice, sizeof(float) * 4 , &subData);

		// create constant buffer for disturbing water wave
		BYTE disturbData[16] = {};
		subData.pSysMem = disturbData;
		m_BufferMap[Buffers::Constant_Disturb] = AddConstBuffer(m_pDevice, 16,&subData);


		ColoredCubeBuffer(this);
		ConstantLightBuffer(this);
		InitBuffer<Buffers::P3_N3_T2_Cube>(this);
		InitBuffer<Buffers::Vertex_P2_T2_Quad>(this);
		InitBuffer<Buffers::Vertex_P3_N3_T2_Quad>(this);
	}
	BufferManager::~BufferManager()
	{
		for (Buffer* buf : m_Buffers)
		{
			delete buf;
		}
	}
	Buffer* BufferManager::AddBuffer(const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData)
	{
		GFX_EXCEPT_SUPPORT();

		m_Buffers.push_back(new Buffer(m_pDevice, pDescript, initData));

		return m_Buffers.back();
	}
	Buffer* BufferManager::AddBuffer(BufferManager::Buffers buffer, const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData)
	{
		if(m_BufferMap.find(buffer) == m_BufferMap.end())
			return m_BufferMap[buffer] = AddBuffer(pDescript, initData);
		else
		{
			FileLogger::Warn("Same buffer is loaded");
			return m_BufferMap[buffer];
		}
	}
	Buffer* BufferManager::GetBuffer(Buffers buffer)
	{
		if (m_BufferMap.find(buffer) == m_BufferMap.end())
		{// The buffer is not loaded.
			FileLogger::Error("The buffer is not loaded.");
			return nullptr;
		}
		return m_BufferMap[buffer];
	}
	IndexBuffer* BufferManager::GetIndexBuffer(BufferManager::Buffers buffer)
	{
		return dynamic_cast<IndexBuffer*>(GetBuffer(buffer));
	}
	VertexBuffer* BufferManager::GetVertexBuffer(BufferManager::Buffers buffer)
	{
		return dynamic_cast<VertexBuffer*>(GetBuffer(buffer));
	}
	ConstBuffer* BufferManager::GetConstBuffer(BufferManager::Buffers buffer)
	{
		return dynamic_cast<ConstBuffer*>(GetBuffer(buffer));
	}
	VertexBuffer* BufferManager::AddVertexBuffer(const Buffer& buffer, size_t offset, unsigned int vertexCount)
	{
		VertexBuffer* vertexBuffer = new VertexBuffer(buffer, offset, vertexCount);
		m_Buffers.push_back(static_cast<Buffer*>(vertexBuffer));
		return vertexBuffer;
	}
	IndexBuffer* BufferManager::AddIndexBuffer(const Buffer& buffer, unsigned int indexCount, unsigned int offset)
	{
		IndexBuffer* indexBuffer = new IndexBuffer(buffer, indexCount, offset);
		m_Buffers.push_back(static_cast<Buffer*>(indexBuffer));
		return indexBuffer;
	}
	VertexBuffer* BufferManager::AddVertexBuffer(const D3D11_BUFFER_DESC* bd, const D3D11_SUBRESOURCE_DATA* sd, size_t offset, unsigned int vertexCount)
	{
		return AddVertexBuffer(Buffer(m_pDevice, bd, sd), offset, vertexCount);
	}
	IndexBuffer* BufferManager::AddIndexBuffer(const D3D11_BUFFER_DESC* bd, const D3D11_SUBRESOURCE_DATA* sd, unsigned int indexCount, unsigned int offset)
	{
		return AddIndexBuffer(Buffer(m_pDevice, bd, sd), indexCount, offset);
	}
	ConstBuffer* BufferManager::AddConstBuffer(ID3D11Device* pDevice, size_t size, const void* data)
	{
		ConstBuffer* constBuffer = new ConstBuffer(pDevice, size, data);
		m_Buffers.push_back(static_cast<Buffer*>(constBuffer));
		return constBuffer;
	}
	ConstBuffer* BufferManager::AddConstBuffer(ID3D11Device* pDevice, size_t size, const D3D11_SUBRESOURCE_DATA* sd)
	{
		ConstBuffer* constBuffer = new ConstBuffer(pDevice, size, sd);
		m_Buffers.push_back(static_cast<Buffer*>(constBuffer));
		return constBuffer;
	}
	VertexBuffer* BufferManager::AddVertexBuffer(Buffers buffers, const Buffer& buffer, size_t offset, unsigned int vertexCount)
	{
		if (m_BufferMap.find(buffers) == m_BufferMap.end())
		{
			VertexBuffer* vertexBuffer = AddVertexBuffer(buffer, offset, vertexCount);
			m_BufferMap[buffers] = static_cast<Buffer*>(vertexBuffer);
			return  vertexBuffer;
		}
		else
		{
			FileLogger::Warn("Same buffer is loaded");
			return dynamic_cast<VertexBuffer*>(m_BufferMap[buffers]);
		}
	}
	IndexBuffer* BufferManager::AddIndexBuffer(Buffers buffers, const Buffer& buffer, unsigned int indexCount, unsigned int offset)
	{
		if (m_BufferMap.find(buffers) == m_BufferMap.end())
		{
			IndexBuffer* indexBuffer = AddIndexBuffer(buffer, indexCount, offset);
			m_BufferMap[buffers] = static_cast<Buffer*>(indexBuffer);
			return indexBuffer;
		}
		else
		{
			FileLogger::Warn("Same buffer is loaded");
			return dynamic_cast<IndexBuffer*>(m_BufferMap[buffers]);
		}
	}
	ConstBuffer* BufferManager::AddConstBuffer(Buffers buffers, ID3D11Device* pDevice, size_t size, const void* data)
	{
		if (m_BufferMap.find(buffers) == m_BufferMap.end())
		{
			ConstBuffer* constBuffer = AddConstBuffer(pDevice, size, data);
			m_BufferMap[buffers] = static_cast<Buffer*>(constBuffer);
			return constBuffer;
		}
		else
		{
			FileLogger::Warn("Same buffer is loaded");
			return dynamic_cast<ConstBuffer*>(m_BufferMap[buffers]);
		}
	}
	ConstBuffer* BufferManager::AddConstBuffer(Buffers buffers, ID3D11Device* pDevice, size_t size, const D3D11_SUBRESOURCE_DATA* sd)
	{
		if (m_BufferMap.find(buffers) == m_BufferMap.end())
		{
			ConstBuffer* constBuffer = AddConstBuffer(pDevice, size, sd);
			m_BufferMap[buffers] = static_cast<Buffer*>(constBuffer);
			return constBuffer;
		}
		else
		{
			FileLogger::Warn("Same buffer is loaded");
			return dynamic_cast<ConstBuffer*>(m_BufferMap[buffers]);
		}
	}
	Buffer* BufferManager::SetBufferData(Buffers buffer, unsigned int size, unsigned int offset, void* src)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		Buffer* buf = GetBuffer(buffer);
		if(buf)
			buf->SetBufferData(pContext, src, size, offset);
		return buf;
	}
}