#include "pch.h"
#include "DSTexture.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{

	DSTexture::DSTexture(ID3D11Device* pDevice, unsigned int width, unsigned int height)
	{
		GFX_EXCEPT_SUPPORT();

		// create texture resource for depth stencil
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;

		InitTextureResource(pDevice, &td, nullptr);

		// create depth stencil view

		GFX_THROW_INFO(pDevice->CreateDepthStencilView(m_pTexture2D.Get(), nullptr, &m_pDSView));

	}
	ID3D11DepthStencilView* DSTexture::GetDSView()
	{
		return m_pDSView.Get();
	}
	void DSTexture::Bind(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::Bind(UINT slot)
	{
	}
	void DSTexture::Unbind(UINT slot)
	{
	}
	void DSTexture::Unbind(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::BindCS(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::BindPS(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::BindGS(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::BindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::UnbindCS(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::UnbindPS(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::UnbindGS(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
	void DSTexture::UnbindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
	}
}
