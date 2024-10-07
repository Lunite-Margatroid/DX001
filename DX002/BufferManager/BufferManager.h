#pragma once
#include <d3d11.h>
#include "Buffer.h"
#include "ConstBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace yoi
{
	class BufferManager
	{
	public:

		/* preloaded buffer */
		enum class Buffers
		{
			// vertex buffer of a cube. Position-float3 : TexCoord-float2
			Vertex_Textured_Cube, 

			// index buffer of a cube. unsigned int * 36
			Index_Textured_Cube,  

			/* 
			   constant buffer for transformation matrix.
			   Model Matrix		 -- float4x4 -- 64B
			   View Matrix		 -- float4x4 -- 64B
			   Projection Matrix -- float4x4 -- 64B
			   MVP Matrix		 -- float4x4 -- 64B
			   Normal Matrix	 -- float4x4 -- 64B
			   Camera Position   -- float3   -- 12B + 4B
			*/
			Constant_Matrix, 

			// vertex buffer of a cube. Position-float3 : Color-ubyte4
			Vertex_Colored_Cube, 

			// index buffer of a cube. unsigned int * 36
			Index_Colored_Cube, 

			// constant buffer for shiniess
			Constant_Material_Shininess,

			// constant buffer for Directional Light
			Constant_DirLight, 

			// constant buffer for Point Light
			Constant_PointLight,

			// constant buffer for Spot Light
			Constant_SpotLight, 

			// vertex buffer of a cube. Position-float3 : Normal-float3 : TexCoord-float2
			P3_N3_T2_Cube,

			// index buffer of a rectangle. It is for triangle strip primitive. unsigned int * 4.
			Index_Quad, 

			// vertex buffer of a 2D rectangle. Position-float2 : TexCoord-float2
			Vertex_P2_T2_Quad,

			// constant buffer for water wave updating
			Constant_Water_Wave_Update,
			
			// constant buffer for disturb wave
			// used by waterwave sprite
			Constant_Disturb,

			// vertex buffer of a 3D rectangle.
			Vertex_P3_N3_T2_Quad
		};


	private:
		std::vector<Buffer*> m_Buffers;
		std::map<BufferManager::Buffers, Buffer*> m_BufferMap;
	public:
		ID3D11Device* m_pDevice;
	public:
		BufferManager(ID3D11Device*);
		BufferManager() = delete;
		~BufferManager();
		BufferManager(const BufferManager&) = delete;
		BufferManager& operator = (const BufferManager&) = delete;

		Buffer* AddBuffer(const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData);
		Buffer* AddBuffer(BufferManager::Buffers buffer, const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData);
		

		VertexBuffer* AddVertexBuffer(const Buffer& buffer, size_t offset, unsigned int vertexCount);
		IndexBuffer* AddIndexBuffer(const Buffer& buffer, unsigned int indexCount, unsigned int offset);

		VertexBuffer* AddVertexBuffer(const D3D11_BUFFER_DESC* bd, const D3D11_SUBRESOURCE_DATA* sd, size_t offset, unsigned int vertexCount);
		IndexBuffer* AddIndexBuffer(const D3D11_BUFFER_DESC* bd, const D3D11_SUBRESOURCE_DATA* sd, unsigned int indexCount, unsigned int offset);

		ConstBuffer* AddConstBuffer(ID3D11Device* pDevice, size_t size, const void* data = nullptr);
		ConstBuffer* AddConstBuffer(ID3D11Device* pDevice, size_t size, const D3D11_SUBRESOURCE_DATA* sd);

		VertexBuffer* AddVertexBuffer(Buffers buffers, const Buffer& buffer, size_t offset, unsigned int vertexCount);
		IndexBuffer* AddIndexBuffer(Buffers buffers, const Buffer& buffer, unsigned int indexCount, unsigned int offset);
		ConstBuffer* AddConstBuffer(Buffers buffers, ID3D11Device* pDevice, size_t size, const void* data = nullptr);
		ConstBuffer* AddConstBuffer(Buffers buffers, ID3D11Device* pDevice, size_t size, const D3D11_SUBRESOURCE_DATA* sd);

		Buffer* GetBuffer(BufferManager::Buffers buffer);
		IndexBuffer* GetIndexBuffer(BufferManager::Buffers buffer);
		VertexBuffer* GetVertexBuffer(BufferManager::Buffers buffer);
		ConstBuffer* GetConstBuffer(BufferManager::Buffers buffer);

		Buffer* SetBufferData(BufferManager::Buffers buffer, unsigned int size, unsigned int offset, void* src);
	};
}