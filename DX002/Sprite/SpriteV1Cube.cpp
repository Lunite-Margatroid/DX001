#include "pch.h"
#include "SpriteV1Cube.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	SpriteV1Cube::SpriteV1Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		:SpriteV1(pDevice, pContext)
	{
		GFX_EXCEPT_SUPPORT();
		// 定义数据
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

		/*Vertex vertexData[] =
		{
			{ -0.5f, -0.5f,0.5f,	0,255,0},
			{ -0.5f, 0.5f, 0.5f,	0,255,0},
			{ 0.5f, 0.5f,  0.5f,	0,255,0},
			{ 0.5f, -0.5f, 0.5f,	0,255,0},

			{-1.0f, -1.0f, -1.0f,	0,255,255},
			{-1.0f, -1.0f, 1.0f,	0,255,255},
			{-1.0f, 1.0f, 1.0f,		0,255,255},
			{-1.0f, 1.0f, -1.0f,	0,255,255},

			{ -1.0f,1.0f, -1.0f,	255,0,255},
			{ 1.0f, 1.0f,-1.0f,		255,0,255},
			{ 1.0f, 1.0f,1.0f,		255,0,255},
			{ -1.0f,1.0f, 1.0f,		255,0,255},

			{ -1.0f,-1.0f, -1.0f,	255,0,0},
			{ -1.0f,-1.0f, 1.0f,	255,0,0},
			{ 1.0f, -1.0f,1.0f,		255,0,0},
			{ 1.0f, -1.0f,-1.0f,	255,0,0},

			{ -0.5f, -0.5f,0.0f,	0,255,0},
			{ -0.5f, 0.5f, 0.0f,	0,255,0},
			{ 0.5f, 0.5f,  0.0f,	0,255,0},
			{ 0.5f, -0.5f, 0.0f,	0,255,0},

			{ -1.0f, -1.0f,-1.0f,	255,255,0},
			{ -1.0f, 1.0f, -1.0f,	255,255,0},
			{ 1.0f, 1.0f, -1.0f,	255,255,0},
			{ 1.0f, -1.0f, -1.0f,	255,255,0},
		};*/

		// 定义数据
		unsigned int indexData[36] = {0,1,2,2,3,0};
		for (int i = 6; i < 36; i++)
		{
			indexData[i] = indexData[i - 6] + 4;
		}

		BYTE constantData[sizeof(float) * 16 * 5];
		memset(constantData, 0, sizeof(float) * 16 * 5);

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

		GFX_THROW_INFO(m_pDevice->CreateBuffer(&bufferDesc, &subData, &m_pVertexBuffer));

		// create index buffer
		bufferDesc.ByteWidth = sizeof(indexData);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = sizeof(unsigned int);

		subData.pSysMem = indexData;
		GFX_THROW_INFO(m_pDevice->CreateBuffer(&bufferDesc, &subData, &m_pIndexBuffer));

		// create constant buffer
		bufferDesc.ByteWidth = sizeof(float) * 4 * 4 * 5;	// 5 * mat4x4
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = 0;
		subData.pSysMem = constantData;
		GFX_THROW_INFO(m_pDevice->CreateBuffer(&bufferDesc, &subData, &m_pConstantBuffer));

		// select shader
		m_Shader = Graphics::GetInstance().GetShader(std::string("Colored Vertex Shader"));
	}
	void SpriteV1Cube::RenderV1(const glm::mat4& modelTrans, const glm::mat4& vpTrans)
	{
		GFX_EXCEPT_SUPPORT();

		glm::mat4 mvpTrans = vpTrans * modelTrans;
		D3D11_MAPPED_SUBRESOURCE dataMap;
		GFX_THROW_INFO(m_pContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &dataMap));
		// memset(dataMap.pData, 0, 64 * 5);
		memcpy((BYTE*)dataMap.pData + 64 * 3, glm::value_ptr(mvpTrans), 64);
		GFX_THROW_INFO_ONLY(m_pContext->Unmap(m_pConstantBuffer.Get(), 0));

		m_Shader->Bind(m_pContext);

		UINT stripe = sizeof(Vertex);
		UINT offset = 0;
		GFX_THROW_INFO_ONLY(m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		GFX_THROW_INFO_ONLY(m_pContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &stripe, &offset));
		GFX_THROW_INFO_ONLY(m_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0));
		GFX_THROW_INFO_ONLY(m_pContext->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf()));
		GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed(36u, 0u, 0u));
		// GFX_THROW_INFO_ONLY(m_pContext->Draw(24u, 0u));
	}
}