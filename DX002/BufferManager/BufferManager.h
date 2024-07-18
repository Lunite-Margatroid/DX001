#pragma once
#include <d3d11.h>

namespace yoi
{
	class BufferManager
	{
	public:
		enum class Buffer
		{
			Vertex_Textured_Cube, Index_Textured_Cube, Constant_Matrix, Vertex_Colored_Cube, Index_Colored_Cube
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
	};
}