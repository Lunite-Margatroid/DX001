#include "pch.h"
#include "UATexture.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
namespace yoi
{
	UATexture::UATexture(const BasicTexture2D& basicTexture)
		:BasicTexture2D(basicTexture)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();

		D3D11_TEXTURE2D_DESC td = {};
		GFX_THROW_INFO_ONLY(m_pTexture2D->GetDesc(&td));

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavd = {};
		uavd.Format = td.Format;
		uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavd.Texture2D.MipSlice = 0u;

		GFX_THROW_INFO(pDevice->CreateUnorderedAccessView(m_pTexture2D.Get(), &uavd, &m_pUAV));
	}
	UATexture::UATexture(ImgRes& img)
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
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sb = {};
		sb.pSysMem = img.GetDataPtr();
		sb.SysMemPitch = img.GetWidth() * img.GetChannal();

		GFX_THROW_INFO(pDevice->CreateTexture2D(&td, &sb, &m_pTexture2D));

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavd = {};
		uavd.Format = td.Format;
		uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavd.Texture2D.MipSlice = 0u;

		GFX_THROW_INFO(pDevice->CreateUnorderedAccessView(m_pTexture2D.Get(), &uavd, &m_pUAV));
	}
	UATexture::UATexture(UINT width, UINT height, void* data, D3D11_USAGE usage)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_R32_FLOAT;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = usage;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data;
		sd.SysMemPitch = width * sizeof(float);
		assert(sizeof(float) == 4);

		GFX_THROW_INFO(pDevice->CreateTexture2D(&td, &sd, &m_pTexture2D));

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavd = {};
		uavd.Format = td.Format;
		uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavd.Texture2D.MipSlice = 0u;

		GFX_THROW_INFO(pDevice->CreateUnorderedAccessView(m_pTexture2D.Get(), &uavd, &m_pUAV));



	}
	void UATexture::Bind(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(slot, 1, m_pUAV.GetAddressOf(), nullptr));
	}
	void UATexture::Bind(UINT slot)
	{
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		Bind(pContext, slot);
	}
	void UATexture::Unbind(UINT slot)
	{
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		Unbind(pContext, slot);
	}
	void UATexture::Unbind(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11UnorderedAccessView *nullPtr = nullptr;
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(slot, 1, &nullPtr, nullptr));
	}

	/************** Compute Shader ******************/
	void UATexture::BindCS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(slot, 1, m_pUAV.GetAddressOf(), nullptr));
	}
	void UATexture::UnbindCS(ID3D11DeviceContext* pContext, UINT slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11UnorderedAccessView* nullPtr = nullptr;
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(slot, 1, &nullPtr, nullptr));
	}

	/************* Pixel Shader ******************/
	void UATexture::BindPS(ID3D11DeviceContext* pContext, UINT slot)
	{
		FileLogger::Error("unodered access view cann't bind to Pixel Shader");
	}
	void UATexture::UnbindPS(ID3D11DeviceContext* pContext, UINT slot)
	{
		FileLogger::Error("unodered access view cann't bind to Pixel Shader");
	}

	/************** Vertex Shader ******************/
	void UATexture::BindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
		FileLogger::Error("unodered access view cann't bind to Vertex Shader");
	}
	void UATexture::UnbindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
		FileLogger::Error("unodered access view cann't bind to Vertex Shader");
	}

	/************** Geometry Shader ******************/
	void UATexture::BindGS(ID3D11DeviceContext* pContext, UINT slot)
	{
		FileLogger::Error("unodered access view cann't bind to Geometry Shader");
	}
	void UATexture::UnbindGS(ID3D11DeviceContext* pContext, UINT slot)
	{
		FileLogger::Error("unodered access view cann't bind to Geometry Shader");
	}

}