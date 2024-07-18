#pragma once
#include "BufferManager.h"
void ColoredCubeBuffer(yoi::BufferManager* bufMng)
{
	struct Vertex
	{
		struct
		{
			float x, y, z;
		}Postion;
		struct
		{
			unsigned char r, g, b, a;
		}Color;
	};

	Vertex vertexData[] =
	{
		{1.0f, -1.0f, -1.0f,	255,255,255},
		{1.0f, -1.0f, 1.0f,		255,255,255},
		{1.0f, 1.0f, 1.0f,		255,255,255},
		{1.0f, 1.0f, -1.0f,		255,255,255},

		{-1.0f, -1.0f, -1.0f,	0,255,255},
		{-1.0f, 1.0f, -1.0f,	0,255,255},
		{-1.0f, 1.0f, 1.0f,		0,255,255},
		{-1.0f, -1.0f, 1.0f,	0,255,255},

		{ -1.0f,1.0f, -1.0f,	255,0,255},
		{ 1.0f, 1.0f,-1.0f,		255,0,255},
		{ 1.0f, 1.0f,1.0f,		255,0,255},
		{ -1.0f,1.0f, 1.0f,		255,0,255},

		{ -1.0f,-1.0f, -1.0f,	255,0,0},
		{ -1.0f,-1.0f, 1.0f,	255,0,0},
		{ 1.0f, -1.0f,1.0f,		255,0,0},
		{ 1.0f, -1.0f,-1.0f,	255,0,0},

		{ -1.f, -1.f,1.0f,	0,255,0},
		{ -1.f, 1.f, 1.0f,	0,255,0},
		{ 1.f, 1.f,  1.0f,	0,255,0},
		{ 1.f, -1.f, 1.0f,	0,255,0},

		{ -1.0f, -1.0f,-1.0f,	255,255,0},
		{ 1.0f, -1.0f, -1.0f,	255,255,0},
		{ 1.0f, 1.0f, -1.0f,	255,255,0},
		{ -1.0f, 1.0f, -1.0f,	255,255,0},
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

	bufMng->AddBuffer(yoi::BufferManager::Buffer::Vertex_Colored_Cube, &bufferDesc, &subData);

	// create index buffer
	bufferDesc.ByteWidth = sizeof(indexData);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.StructureByteStride = sizeof(unsigned int);

	subData.pSysMem = indexData;
	bufMng->AddBuffer(yoi::BufferManager::Buffer::Index_Colored_Cube, &bufferDesc, &subData);
}