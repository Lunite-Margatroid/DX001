#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ImgRes.h"
#include "BasicTexture2D.h"
namespace yoi
{
	class Texture : public BasicTexture2D
	{
		friend class Material;
		friend class TextureManager;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pResView;
	public:
		Texture(const ImgRes& img);
		// Create a single-color Texture
		Texture(const float* color);
		Texture(ID3D11Device*, const D3D11_TEXTURE2D_DESC*, const D3D11_SUBRESOURCE_DATA*);
		Texture(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture);
		~Texture() = default;
		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;

		void Bind(UINT resigter = 0u) override;
		void Bind(ID3D11DeviceContext* pContext, UINT resigter = 0u) override;
		void Unbind(UINT slot = 0u) override;
		void Unbind(ID3D11DeviceContext* pContext, UINT slot = 0u) override;

		void BindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		void BindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		void BindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		void BindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;

		void UnbindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		void UnbindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		void UnbindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		void UnbindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;


	};
}