#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace yoi
{
	class Buffer
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dBuffer;

	public:
		Buffer();
		Buffer(const Buffer&);
		Buffer(Buffer&&) noexcept;
		Buffer& operator = (const Buffer&);
		Buffer& operator = (Buffer&&) noexcept;
		virtual ~Buffer() = default;

		void Init(ID3D11Device* pDevice, const D3D11_BUFFER_DESC* bd, const D3D11_SUBRESOURCE_DATA* sd);
		ID3D11Buffer* GetBuffer();
	};
}