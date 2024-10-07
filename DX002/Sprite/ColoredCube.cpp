#include "pch.h"
#include "ColoredCube.h"
#include "Renderer\Graphics.h"

namespace yoi
{
	ColoredCube::ColoredCube()
	{
		m_VertexBuffers.push_back(Graphics::GetInstance().GetVertexBuffer(BufferManager::Buffers::Vertex_Colored_Cube));
		m_pIndexBuffer = Graphics::GetInstance().GetIndexBuffer(BufferManager::Buffers::Index_Colored_Cube);

		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		m_Shader = Graphics::GetInstance().GetShader("Colored Vertex Shader");
	}
}