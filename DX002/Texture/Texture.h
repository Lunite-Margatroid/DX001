#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ImgRes.h"
namespace yoi
{
	class Texture
	{
		friend class Material;
		friend class TextureManager;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pResView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture2D;
		unsigned int m_Width;
		unsigned int m_Height;
	public:
		Texture(const ImgRes& img);
		// Create a single-color Texture
		Texture(const float* color);
		Texture(ID3D11Device*, const D3D11_TEXTURE2D_DESC*, const D3D11_SUBRESOURCE_DATA*);
		~Texture() = default;
		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;

		void Bind(UINT resigter = 0u);
		void Bind(ID3D11DeviceContext* pContext, UINT resigter = 0u);
		void BindCS(ID3D11DeviceContext* pContext, UINT resigter = 0u);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture2D();

	};
}