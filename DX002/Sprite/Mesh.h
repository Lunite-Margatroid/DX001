#pragma once
#include "BufferManager\BufferManager.h"
#include "Shader\Shader.h"
#include "Material/MaterialManager.h"
#include "BufferManager\VertexBuffer.h"
#include "BufferManager\IndexBuffer.h"

namespace yoi
{
	class Mesh
	{
	protected:
		std::vector<VertexBuffer*> m_VertexBuffers;
		IndexBuffer* m_pIndexBuffer;

		D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;

		Shader* m_Shader;
		Material* m_Material;

		Mesh();
	public:
		
		Mesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer,  D3D11_PRIMITIVE_TOPOLOGY primitive, Shader* shader, Material* material = nullptr);
		Mesh(Mesh&&) noexcept;
		~Mesh() = default;
		Mesh& operator = (const Mesh&) = default;
		Mesh(const Mesh&) = default;

		void Draw(ID3D11DeviceContext* pContext);
		void SetMaterial(Material* material);
	};
}