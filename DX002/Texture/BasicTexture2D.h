#pragma once
#include <d3d11.h>
#include <wrl.h>
namespace yoi
{
	class BasicTexture2D
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture2D;

	public:
		BasicTexture2D() = default;
		virtual ~BasicTexture2D() = default;
		BasicTexture2D(const BasicTexture2D&);
		BasicTexture2D& operator = (const BasicTexture2D&);

		BasicTexture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture);
		BasicTexture2D(const D3D11_TEXTURE2D_DESC* td, const D3D11_SUBRESOURCE_DATA* sd);


		void InitTextureResource(const D3D11_TEXTURE2D_DESC* td, const D3D11_SUBRESOURCE_DATA* sd);
		

		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture2D();
		ID3D11Texture2D* GetTexture2DPtr();
		/********* defulat bind func ****************/
		
		virtual void Bind(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;
		virtual void Bind(UINT slot = 0u) = 0;
		virtual void Unbind(UINT slot = 0u) = 0;
		virtual void Unbind(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;

		/******************************************/

		virtual void BindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;
		virtual void BindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;
		virtual void BindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;
		virtual void BindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;

		virtual void UnbindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;
		virtual void UnbindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;
		virtual void UnbindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;
		virtual void UnbindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) = 0;
	};
}