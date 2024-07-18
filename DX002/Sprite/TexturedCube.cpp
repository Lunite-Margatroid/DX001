#include "pch.h"
#include "TexturedCube.h"
#include "Renderer\Graphics.h"

#define GET_BUFFER(buffer) Graphics::GetInstance().GetBuffer((buffer))

namespace yoi
{
	TexturedCube::TexturedCube()
	{
		m_pVertexBuffer = GET_BUFFER(BufferManager::Buffer::Vertex_Textured_Cube);
		m_pIndexBuffer = GET_BUFFER(BufferManager::Buffer::Index_Textured_Cube);

		m_VertexStripe = sizeof(float) * 5;
		m_VertexOffset = 0u;

		m_IndexOffset = 0u;
		m_IndexCount = 36u;

		m_StartIndex = 0u;
		m_BaseIndex = 0u;

		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		m_Shader = Graphics::GetInstance().GetShader("Textured Shader");
	}
}