#include "pch.h"
#include "ColoredCube.h"
#include "Renderer\Graphics.h"

namespace yoi
{
	ColoredCube::ColoredCube()
	{
		m_pVertexBuffer = Graphics::GetInstance().GetBuffer(BufferManager::Buffer::Vertex_Colored_Cube);
		m_pIndexBuffer = Graphics::GetInstance().GetBuffer(BufferManager::Buffer::Index_Colored_Cube);

		m_VertexStripe = sizeof(float) * 3 + 4;
		m_VertexOffset = 0u;

		m_IndexOffset = 0u;
		m_IndexCount = 36u;

		m_StartIndex = 0u;
		m_BaseIndex = 0u;

		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		m_Shader = Graphics::GetInstance().GetShader("Colored Vertex Shader");
	}
}