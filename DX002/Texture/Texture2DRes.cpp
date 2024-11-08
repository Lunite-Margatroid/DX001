#include "pch.h"
#include "Texture2DRes.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
namespace yoi
{
    Texture2DRes::Texture2DRes(const ImgRes& res)
    {
		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = res.GetWidth();
		td.Height = res.GetHeight();
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sb = {};
		sb.pSysMem = res.GetDataPtr();
		sb.SysMemPitch = res.GetWidth() * res.GetChannal();

		GFX_THROW_INFO(pDevice->CreateTexture2D(&td, &sb, &m_pTexture2D));
    }
    Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2DRes::GetTexture2D()
    {
        return m_pTexture2D;
    }
    ID3D11Texture2D* Texture2DRes::GetTexture2DPtr()
    {
        return m_pTexture2D.Get();
    }
	
}