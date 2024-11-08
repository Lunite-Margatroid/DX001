#include "pch.h"
#include "CubeTexture.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	void CubeTexture::Init(ID3D11Device* pDevcie, const D3D11_TEXTURE2D_DESC* td, const D3D11_SUBRESOURCE_DATA* sd)
	{
		GFX_EXCEPT_SUPPORT();

		GFX_THROW_INFO(pDevcie->CreateTexture2D(td, sd, &m_pTexture2D));

		D3D11_SHADER_RESOURCE_VIEW_DESC rvd = {};
		rvd.Format = td->Format;
		rvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		rvd.TextureCube.MipLevels = 1u;
		rvd.TextureCube.MostDetailedMip = 0u;
		GFX_THROW_INFO(pDevcie->CreateShaderResourceView(m_pTexture2D.Get(), &rvd, &m_pResView));
	}
	CubeTexture::CubeTexture(ID3D11Device* pDevice, unsigned int width, unsigned int height)
	{
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1u;
		td.ArraySize = 6u;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		td.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		Init(pDevice, &td, nullptr);

	}
	void CubeTexture::SetFaceTexture(ID3D11DeviceContext* pContext, D3D11_TEXTURECUBE_FACE face, ImgRes& img)
	{
		assert(img.GetChannal() == 4);
		D3D11_TEXTURE2D_DESC td;
		m_pTexture2D->GetDesc(&td);
		if (img.GetHeight() != td.Height || img.GetWidth() != td.Width)
		{
			img.Resize(td.Width, td.Height);
		}
		std::unique_ptr<Texture2DRes> tempTex = std::make_unique<Texture2DRes>(img);
		pContext->CopySubresourceRegion(
			m_pTexture2D.Get(), CalcSubSrc(0, face, td.MipLevels), 0, 0, 0,
			tempTex->GetTexture2DPtr(), 0, nullptr);
	}
}