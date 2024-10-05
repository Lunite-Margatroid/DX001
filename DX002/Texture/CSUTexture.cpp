#include "pch.h"
#include "CSUTexture.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	CSUTexture::CSUTexture(const ImgRes& imgRes)
	{
		// initialize texture2d resource
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = imgRes.GetWidth();
		td.Height = imgRes.GetHeight();
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		td.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		td.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = imgRes.GetDataPtr();
		sd.SysMemPitch = imgRes.GetChannal() * imgRes.GetWidth();

		InitTextureResource(&td, &sd);

		// create views
		InitResourceView();
	}

	CSUTexture::CSUTexture(unsigned int width, unsigned int height, DXGI_FORMAT format, UINT cpuAccess)
	{
		assert(format == DXGI_FORMAT_R32_FLOAT || format == DXGI_FORMAT_R32G32B32_FLOAT);
		int nChannal = format == DXGI_FORMAT_R32_FLOAT ? 1 : 3;

		// temp data
		std::unique_ptr<float[]> tData( new float[width * height * nChannal]);
		memset(tData.get(), 0, width * height * sizeof(float) * nChannal);

		// initialize texture2d resource
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = format;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		td.CPUAccessFlags = cpuAccess;
		td.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = tData.get();
		sd.SysMemPitch = width * sizeof(float) * nChannal;

		InitTextureResource(&td, &sd);

		// create views
		InitResourceView();
	}

	void CSUTexture::InitResourceView()
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_TEXTURE2D_DESC td = {};
		GFX_THROW_INFO_ONLY(m_pTexture2D->GetDesc(&td));

		// create shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1u;
		srvd.Texture2D.MostDetailedMip = 0u;

		GFX_THROW_INFO(pDevice->CreateShaderResourceView(m_pTexture2D.Get(), &srvd, &m_pResView));

		// create unordered access view
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavd = {};
		uavd.Format = td.Format;
		uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavd.Texture2D.MipSlice = 0u;

		GFX_THROW_INFO(pDevice->CreateUnorderedAccessView(m_pTexture2D.Get(), &uavd, &m_pUAV));
	}

	/**************** default binder **********************/
	void CSUTexture::Bind(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(slot, 1, m_pResView.GetAddressOf()));
	}
	void CSUTexture::Bind(UINT slot)
	{
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		Bind(pContext, slot);
	}
	void CSUTexture::Unbind(UINT slot)
	{
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		Unbind(pContext, slot);
	}
	void CSUTexture::Unbind(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11ShaderResourceView* nullPtr = nullptr;
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(slot, 1, &nullPtr));
	}

	/************** Compute Shader ******************/
	void CSUTexture::BindCS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(slot, 1, m_pUAV.GetAddressOf(), nullptr));
	}
	void CSUTexture::UnbindCS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11UnorderedAccessView* nullPtr = nullptr;
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(slot, 1, &nullPtr, nullptr));
	}

	/************* Pixel Shader ******************/
	void CSUTexture::BindPS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(slot, 1u, m_pResView.GetAddressOf()));
	}
	void CSUTexture::UnbindPS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11ShaderResourceView* null_ptr = nullptr;
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(slot, 1u, &null_ptr));
	}

	/************** Vertex Shader ******************/
	void CSUTexture::BindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->VSSetShaderResources(slot, 1u, m_pResView.GetAddressOf()));
	}
	void CSUTexture::UnbindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11ShaderResourceView* null_ptr = nullptr;
		GFX_THROW_INFO_ONLY(pContext->VSSetShaderResources(slot, 1u, &null_ptr));
	}

	/************** Geometry Shader ******************/
	void CSUTexture::BindGS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->GSSetShaderResources(slot, 1u, m_pResView.GetAddressOf()));
	}
	void CSUTexture::UnbindGS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11ShaderResourceView* null_ptr = nullptr;
		GFX_THROW_INFO_ONLY(pContext->GSSetShaderResources(slot, 1u, &null_ptr));
	}
}