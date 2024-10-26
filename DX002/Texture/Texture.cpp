#include "pch.h"
#include "Texture.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
namespace yoi
{
	void Texture::Init(ID3D11Device* pDevcie, const D3D11_TEXTURE2D_DESC* td, const D3D11_SUBRESOURCE_DATA* sd)
	{
		GFX_EXCEPT_SUPPORT();

		GFX_THROW_INFO(pDevcie->CreateTexture2D(td, sd, &m_pTexture2D));

		D3D11_SHADER_RESOURCE_VIEW_DESC rvd = {};
		rvd.Format = td->Format;
		rvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		rvd.Texture2D.MostDetailedMip = 0u;
		rvd.Texture2D.MipLevels = 1u;
		GFX_THROW_INFO(pDevcie->CreateShaderResourceView(m_pTexture2D.Get(), &rvd, &m_pResView));
	}
	Texture::Texture(const ImgRes& img)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = img.GetWidth();
		td.Height = img.GetHeight();
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
		sb.pSysMem = img.GetDataPtr();
		sb.SysMemPitch = img.GetWidth() * img.GetChannal();

		GFX_THROW_INFO(pDevice->CreateTexture2D(&td, &sb, &m_pTexture2D));

		D3D11_SHADER_RESOURCE_VIEW_DESC rvd = {};
		rvd.Format = td.Format;
		rvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		rvd.Texture2D.MostDetailedMip = 0u;
		rvd.Texture2D.MipLevels = 1u;
		GFX_THROW_INFO(pDevice->CreateShaderResourceView(m_pTexture2D.Get(), &rvd, &m_pResView));
	}

	Texture::Texture(ID3D11Device* device, const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* data)
	{
		Init(device, desc, data);
	}

	Texture::Texture(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture)
		:BasicTexture2D(pTexture)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_TEXTURE2D_DESC  td = {};
		GFX_THROW_INFO_ONLY(pTexture->GetDesc(&td));
		D3D11_SHADER_RESOURCE_VIEW_DESC rvd = {};
		rvd.Format = td.Format;
		rvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		rvd.Texture2D.MostDetailedMip = 0u;
		rvd.Texture2D.MipLevels = 1u;
		GFX_THROW_INFO(pDevice->CreateShaderResourceView(m_pTexture2D.Get(), &rvd, &m_pResView));
	}

	Texture::Texture(const float* color)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = 1u;
		td.Height =  1u;
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

		InitTextureResource(&td, &sb);

		D3D11_SHADER_RESOURCE_VIEW_DESC rvd = {};
		rvd.Format = td.Format;
		rvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		rvd.Texture2D.MostDetailedMip = 0u;
		rvd.Texture2D.MipLevels = 1u;
		GFX_THROW_INFO(pDevice->CreateShaderResourceView(m_pTexture2D.Get(), &rvd, &m_pResView));
	}

	/********* default bind *************/
	void Texture::Bind(UINT resigter )
	{
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		pContext->PSSetShaderResources(resigter, 1u, m_pResView.GetAddressOf());
	}
	
	void Texture::Bind(ID3D11DeviceContext* pContext, UINT resigter )
	{
		pContext->PSSetShaderResources(resigter, 1u, m_pResView.GetAddressOf());
	}

	void Texture::Unbind(UINT slot)
	{
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		ID3D11ShaderResourceView* null_ptr = nullptr;
		pContext->PSSetShaderResources(slot, 1u, &null_ptr);
	}

	void Texture::Unbind(ID3D11DeviceContext* pContext, UINT slot)
	{
		ID3D11ShaderResourceView* null_ptr = nullptr;
		pContext->PSSetShaderResources(slot, 1u, &null_ptr);
	}

	/********* bind compute shader *********/
	void Texture::BindCS(ID3D11DeviceContext* pContext, UINT slot)
	{
		pContext->CSSetShaderResources(slot, 1u, m_pResView.GetAddressOf());
	}

	void Texture::UnbindCS(ID3D11DeviceContext* pContext, UINT slot)
	{
		ID3D11ShaderResourceView* null_ptr = nullptr;
		pContext->CSSetShaderResources(slot, 1u, &null_ptr);
	}

	/*********** bind vertex shader ****************/
	void Texture::BindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
		pContext->VSSetShaderResources(slot, 1u, m_pResView.GetAddressOf());
	}

	void Texture::UnbindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
		ID3D11ShaderResourceView* null_ptr = nullptr;
		pContext->VSSetShaderResources(slot, 1u, &null_ptr);
	}

	/*********** bind pixel shader *****************/
	void Texture::BindPS(ID3D11DeviceContext* pContext, UINT slot)
	{
		pContext->PSSetShaderResources(slot, 1u, m_pResView.GetAddressOf());
	}

	void Texture::UnbindPS(ID3D11DeviceContext* pContext, UINT slot)
	{
		ID3D11ShaderResourceView* null_ptr = nullptr;
		pContext->PSSetShaderResources(slot, 1u, &null_ptr);
	}

	/*********** bind geometry shader ******************/
	void Texture::BindGS(ID3D11DeviceContext* pContext, UINT slot)
	{
		pContext->GSSetShaderResources(slot, 1u, m_pResView.GetAddressOf());
	}

	void Texture::UnbindGS(ID3D11DeviceContext* pContext, UINT slot)
	{
		ID3D11ShaderResourceView* null_ptr = nullptr;
		pContext->GSSetShaderResources(slot, 1u, &null_ptr);
	}
}