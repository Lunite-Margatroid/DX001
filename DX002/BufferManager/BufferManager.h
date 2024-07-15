#pragma once
#include <d3d11.h>

namespace yoi
{
	class BufferManager
	{
	private:
		std::vector<ID3D11Buffer*> m_Buffers;
		ID3D11Device* m_pDevice;
	public:
		BufferManager(ID3D11Device*);
		~BufferManager();
		BufferManager(const BufferManager&) = delete;
		BufferManager& operator = (const BufferManager&) = delete;

		ID3D11Buffer* AddBuffer(const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData);
	};
}