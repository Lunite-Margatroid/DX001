#pragma once
#include <d3d11.h>

namespace yoi
{
	class BufferManager
	{
	public:

		/* preloaded buffer */
		enum class Buffer
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

			// index buffer of a rectangle. It is for trangle strip primitive. unsigned int * 4.
			Index_Quad, 

			// vertex buffer of a 2D rectangle. Position-float2 : TexCoord-float2
			Vertex_P2_T2_Quad,

			// constant buffer for water wave updating
			Constant_Water_Wave_Update
		};


	private:
		std::vector<ID3D11Buffer*> m_Buffers;
		ID3D11Device* m_pDevice;
		std::map<Buffer, ID3D11Buffer*> m_BufferMap;
	public:
		BufferManager(ID3D11Device*);
		BufferManager() = delete;
		~BufferManager();
		BufferManager(const BufferManager&) = delete;
		BufferManager& operator = (const BufferManager&) = delete;

		ID3D11Buffer* AddBuffer(const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData);
		ID3D11Buffer* AddBuffer(Buffer buffer, const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData);
		ID3D11Buffer* GetBuffer(Buffer buffer);

		ID3D11Buffer* SetBufferData(Buffer buffer, unsigned int size, unsigned int offset, void* src);
	};
}