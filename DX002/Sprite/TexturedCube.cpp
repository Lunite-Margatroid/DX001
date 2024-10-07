#include "pch.h"
#include "TexturedCube.h"
#include "Renderer\Graphics.h"

#define GET_BUFFER(buffer) Graphics::GetInstance().GetBuffer((buffer))

namespace yoi
{
	TexturedCube::TexturedCube()
	{
		m_VertexBuffers.push_back( Graphics::GetInstance().GetVertexBuffer( BufferManager::Buffers::Vertex_Textured_Cube));
		m_pIndexBuffer = Graphics::GetInstance().GetIndexBuffer(BufferManager::Buffers::Index_Textured_Cube);

		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		m_Shader = Graphics::GetInstance().GetShader("Textured Shader");
	}
}