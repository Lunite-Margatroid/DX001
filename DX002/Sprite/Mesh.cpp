#include "pch.h"
#include "Mesh.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	Mesh::Mesh()
		:m_pVertexBuffer(nullptr),
		m_pIndexBuffer(nullptr),
		m_VertexStripe(0u),
		m_VertexOffset(0u),
		m_IndexOffset(0u),
		m_IndexCount(0u),
		m_StartIndex(0u),
		m_BaseIndex(0u),
		m_PrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED),
		m_Shader(nullptr),
		m_Material(nullptr)
	{
	}
	Mesh::Mesh(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT stripe, UINT offset, UINT indexOffset, UINT count, UINT start, UINT base, D3D11_PRIMITIVE_TOPOLOGY primitive, Shader* shader, Material* material)
		:m_pVertexBuffer(vertexBuffer),
		m_pIndexBuffer(indexBuffer),
		m_VertexStripe(stripe),
		m_VertexOffset(offset),
		m_IndexOffset(indexOffset),
		m_IndexCount(count),
		m_StartIndex(start),
		m_BaseIndex(base),
		m_PrimitiveTopology(primitive),
		m_Shader(shader),
		m_Material(material)
	{
	}
	void Mesh::Draw(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();
		m_Shader->Bind(pContext);
		if(m_Material)
			m_Material->Bind(pContext);
		GFX_THROW_INFO_ONLY(pContext->IASetPrimitiveTopology(m_PrimitiveTopology));
		GFX_THROW_INFO_ONLY(pContext->IASetVertexBuffers(0u, 1u, &m_pVertexBuffer, &m_VertexStripe, &m_VertexOffset));
		GFX_THROW_INFO_ONLY(pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, m_IndexOffset));
		GFX_THROW_INFO_ONLY(pContext->DrawIndexed(m_IndexCount, m_StartIndex, m_BaseIndex));
		
	}
	void Mesh::SetMaterial(Material* material)
	{
		m_Material = material;
	}
}