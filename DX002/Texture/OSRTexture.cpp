#include "pch.h"
#include "OSRTexture.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	OSRTexture::OSRTexture(ID3D11Device* pDevice, unsigned int width, unsigned int height, DXGI_FORMAT format)
	{
		D3D11_TEXTURE2D_DESC td = {};

		td.Format = format;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.ArraySize = 1;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags  = 0u;
		td.Width = width;
		td.Height = height;
		td.MipLevels = 0u;
		td.MiscFlags = 0u;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;

		size_t sizePixel = 4;
		switch (format)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			sizePixel = 16;
			break;
		case DXGI_FORMAT_R16G16B16A16_UNORM:
			sizePixel = 8; 
			break;
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			sizePixel = 4;
			break;
		default:break;
		}
		
		std::unique_ptr<BYTE[]> tempData(new BYTE[sizePixel * width * height]);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = tempData.get();
		sd.SysMemPitch = width * sizePixel;
		sd.SysMemSlicePitch = 0u;

		Texture::Init(pDevice, &td, &sd);

		D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
		rtvd.Format = format;
		rtvd.Texture2D.MipSlice = 0u;

		pDevice->CreateRenderTargetView(m_pTexture2D.Get(), &rtvd, &m_RenderTargetView);

	}
	void OSRTexture::BindRenderTarget(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr));
	}
}