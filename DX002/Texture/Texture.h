#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ImgRes.h"
namespace yoi
{
	class Texture
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pResView;
	public:
		Texture(const ImgRes& img);
		~Texture() = default;
		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;

		void Bind();
		void Bind(ID3D11DeviceContext* pContext);
	};
}