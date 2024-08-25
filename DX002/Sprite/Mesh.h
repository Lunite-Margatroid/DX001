#pragma once
#include "BufferManager\BufferManager.h"
#include "Shader\Shader.h"
#include "Material/MaterialManager.h"

namespace yoi
{
	class Mesh
	{
	protected:
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;

		UINT m_VertexStripe;
		UINT m_VertexOffset;

		UINT m_IndexOffset;

		UINT m_IndexCount;
		UINT m_StartIndex;
		UINT m_BaseIndex;

		D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;

		Shader* m_Shader;
		Material* m_Material;

		Mesh();
	public:
		/// <summary>
		/// yoi mesh
		/// </summary>
		/// <param name="vertexBuffer"> ptr of vertex buffer </param>
		/// <param name="indexBuffer"> ptr of index buffer </param>
		/// <param name="stripe"> byte stripe of buffer of per vertex </param>
		/// <param name="offset"> byte offset of vertex buffer </param>
		/// <param name="indexOffset"> count offset of index buffer </param>
		/// <param name="count"> count of valid index </param>
		/// <param name="start"> offset of first index </param>
		/// <param name="base"> base of index buffer </param>
		/// <param name="primitive"> primitive geometry </param>
		/// <param name="shader"> the shader to render the mesh </param>
		/// <param name="material"> the material for the mesh</param>
		Mesh(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer,  UINT stripe,UINT offset,UINT indexOffset, UINT count, UINT start, UINT base, D3D11_PRIMITIVE_TOPOLOGY primitive, Shader* shader, Material* material = nullptr);
		~Mesh() = default;
		Mesh& operator = (const Mesh&) = default;
		Mesh(const Mesh&) = default;

		void Draw(ID3D11DeviceContext* pContext);
		void SetMaterial(Material* material);
	};
}