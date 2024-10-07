#pragma once
#include "Buffer.h"

namespace yoi
{
	class ConstBuffer : public Buffer
	{
	public:
		ConstBuffer(ID3D11Device* pDevice, size_t size, const void* data = nullptr);
		ConstBuffer(ID3D11Device* pDevice, size_t size, const D3D11_SUBRESOURCE_DATA* sd);
		ConstBuffer() = default;
		ConstBuffer(const ConstBuffer&) = delete;
		ConstBuffer& operator = (const ConstBuffer&) = delete;

		void Bind(ID3D11DeviceContext* pContext, unsigned int slot = 0u);
		void BindVS(ID3D11DeviceContext* pContext, unsigned int slot = 0u);
		void BindGS(ID3D11DeviceContext* pContext, unsigned int slot = 0u);
		void BindCS(ID3D11DeviceContext* pContext, unsigned int slot = 0u);
		void BindPS(ID3D11DeviceContext* pContext, unsigned int slot = 0u);

	
	};
}