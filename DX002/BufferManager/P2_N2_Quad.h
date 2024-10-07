#pragma once

namespace yoi
{
	// Init vertex_buffer and index_buffer of textured quad
	template<>
	void InitBuffer<BufferManager::Buffers::Vertex_P2_T2_Quad>(yoi::BufferManager* bufMng)
	{
		struct Vertex
		{
			struct
			{
				float x, y;
			}Postion;
			struct
			{
				float u, v;
			}TexCoord;
		};

		Vertex vertexData[] =
		{
			{{-1.0f, -1.0f}, {0.0f, 1.0f}},
			{{1.0f, -1.0f}, {1.0f, 1.0f}},
			{{1.0f, 1.0f}, {1.0f, 0.0f}},
			{{-1.0f, 1.0f}, {0.0f, 0.0f}}
		};

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

		bufMng->AddVertexBuffer(yoi::BufferManager::Buffers::Vertex_P2_T2_Quad,yoi::Buffer(bufMng->m_pDevice,&bufferDesc, &subData), 
			0, 4)
			->SetLayout<VertexBuffer::Distrib<float, 2>, VertexBuffer::Distrib<float, 2>>();


		// create index buffer

		unsigned int indexData[4] = { 0, 1, 3, 2 };

		bufferDesc.ByteWidth = sizeof(indexData);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = sizeof(unsigned int);

		subData.pSysMem = indexData;
		bufMng->AddIndexBuffer(yoi::BufferManager::Buffers::Index_Quad, yoi::Buffer(bufMng->m_pDevice, &bufferDesc, &subData), 4, 0);
	}
}