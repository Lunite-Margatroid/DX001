#include "pch.h"
#include "Texture.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
namespace yoi
{
	Texture::Texture(const ImgRes& img)
	{
		GFX_EXCEPT_SUPPORT();
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = m_Width = img.GetWidth();
		td.Height = m_Height = img.GetHeight();
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sb = {};
		sb.pSysMem = img.GetDataPtr();
		sb.SysMemPitch = img.GetWidth() * img.GetChannal();

		GFX_THROW_INFO(pDevice->CreateTexture2D(&td, &sb, &pTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC rvd = {};
		rvd.Format = td.Format;
		rvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		rvd.Texture2D.MostDetailedMip = 0u;
		rvd.Texture2D.MipLevels = 1u;
		GFX_THROW_INFO(pDevice->CreateShaderResourceView(pTexture.Get(), &rvd, &m_pResView));
	}
	Texture::Texture(const float* color)
	{
		GFX_EXCEPT_SUPPORT();
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = m_Width = 1u;
		td.Height = m_Height = 1u;
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sb = {};
		sb.pSysMem = color;
		sb.SysMemPitch = sizeof(float) * 4;

		GFX_THROW_INFO(pDevice->CreateTexture2D(&td, &sb, &pTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC rvd = {};
		rvd.Format = td.Format;
		rvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		rvd.Texture2D.MostDetailedMip = 0u;
		rvd.Texture2D.MipLevels = 1u;
		GFX_THROW_INFO(pDevice->CreateShaderResourceView(pTexture.Get(), &rvd, &m_pResView));
	}
	void Texture::Bind()
	{
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		pContext->PSSetShaderResources(0u, 1u, m_pResView.GetAddressOf());
	}
	void Texture::Bind(ID3D11DeviceContext* pContext)
	{
		pContext->PSSetShaderResources(0u, 1u, m_pResView.GetAddressOf());
	}
}