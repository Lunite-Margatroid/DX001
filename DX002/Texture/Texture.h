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

		Texture() = default;

		void Init(ID3D11Device* pDevcie, const D3D11_TEXTURE2D_DESC* td, const D3D11_SUBRESOURCE_DATA* sd);
	public:
		Texture(const ImgRes& img);
		// Create a single-color Texture
		Texture(const float* color);
		Texture(ID3D11Device*, const D3D11_TEXTURE2D_DESC*, const D3D11_SUBRESOURCE_DATA*);
		Texture(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture);
		~Texture() = default;
		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;

		virtual void Bind(UINT resigter = 0u) override;
		virtual void Bind(ID3D11DeviceContext* pContext, UINT resigter = 0u) override;
		virtual void Unbind(UINT slot = 0u) override;
		virtual void Unbind(ID3D11DeviceContext* pContext, UINT slot = 0u) override;

		virtual void BindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;

		virtual void UnbindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;


	};
}