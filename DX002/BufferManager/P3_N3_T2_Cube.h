#pragma once

namespace yoi
{
	template<>
	void InitBuffer<BufferManager::Buffer::P3_N3_T2_Cube>(yoi::BufferManager* bufMng)
	{
		struct Vertex
		{
			struct
			{
				float x, y, z;
			}Postion;
			struct
			{
				float x, y, z;
			}Normal;
			struct
			{
				float u, v;
			}TexCoord;
		};

		Vertex vertexData[] =
		{
			{1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f},
			{1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f},
			{1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f},
			{1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f},

			{-1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f},
			{-1.0f, 1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f},
			{-1.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f},
			{-1.0f, -1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 1.0f},

			{ -1.0f,1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f},
			{ 1.0f, 1.0f,-1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f},
			{ 1.0f, 1.0f,1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f},
			{ -1.0f,1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f},

			{ -1.0f,-1.0f, -1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f},
			{ -1.0f,-1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f},
			{ 1.0f, -1.0f,1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f},
			{ 1.0f, -1.0f,-1.0f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f},

			{ -1.f, -1.f,1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f},
			{ -1.f, 1.f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f},
			{ 1.f, 1.f,  1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f},
			{ 1.f, -1.f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f},

			{ -1.0f, -1.0f,-1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f},
			{ 1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f},
			{ 1.0f, 1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f},
			{ -1.0f, 1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f},
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

		bufMng->AddBuffer(yoi::BufferManager::Buffer::P3_N3_T2_Cube, &bufferDesc, &subData);
	}
}