#include "pch.h"
#include "Mesh.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	Mesh::Mesh()
		:m_pIndexBuffer(nullptr),
		m_PrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED),
		m_Shader(nullptr),
		m_Material(nullptr)
	{
	}
	Mesh::Mesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, D3D11_PRIMITIVE_TOPOLOGY primitive, Shader* shader, Material* material)
		:
		m_pIndexBuffer(indexBuffer),
		m_PrimitiveTopology(primitive),
		m_Shader(shader),
		m_Material(material)
	{
		m_VertexBuffers.push_back(vertexBuffer);
	}
	Mesh::Mesh(Mesh&& other) noexcept
		:m_VertexBuffers(std::move(other.m_VertexBuffers)),
		m_pIndexBuffer(other.m_pIndexBuffer),
		m_PrimitiveTopology(other.m_PrimitiveTopology),
		m_Shader(other.m_Shader),
		m_Material(other.m_Material)
	{
		
	}
	void Mesh::Draw(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();
		m_Shader->Bind(pContext);
		if(m_Material)
			m_Material->Bind(pContext);
		GFX_THROW_INFO_ONLY(pContext->IASetPrimitiveTopology(m_PrimitiveTopology));

		unsigned int startSlot = 0;
		for (VertexBuffer* vertBuffer : m_VertexBuffers)
		{
			startSlot += vertBuffer->Bind(pContext, startSlot);
		}
		GFX_THROW_INFO_ONLY(pContext->IASetIndexBuffer(m_pIndexBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, m_pIndexBuffer->GetOffset() * sizeof(unsigned int)));
		GFX_THROW_INFO_ONLY(pContext->DrawIndexed(m_pIndexBuffer->GetIndexCount(), 0u, 0u));
		if (m_Material)
			m_Material->Unbind(pContext);
	}
	void Mesh::SetMaterial(Material* material)
	{
		m_Material = material;
	}
}