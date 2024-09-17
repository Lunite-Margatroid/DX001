#include "pch.h"
#include "Sampler.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	Sampler::Sampler(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE wrap, float* borderColor)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();

		/*********** init sampler desc ****************/

		D3D11_SAMPLER_DESC sd = {};
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// the way of filter
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	// wrap method along width
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;	// wrap method along height
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;	// wrap method along depth
		sd.MipLODBias = 0.0f;						// the offset when selecting mipmap level
		sd.MinLOD = 0.0f;							// the highest quality(lowest level) when selecting mipmap level
		sd.MaxLOD = D3D11_FLOAT32_MAX;				// the lowest quality(highest level) when selecting mipmap level
		sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;// a comparison func like depth or stencil test. But how the func works? Maybe it is for D3D11_FILTER_COMPARISON_ANISOTROPIC.
		
		// level of anisotropy filter(各向异性采样)

		if (filter == D3D11_FILTER_ANISOTROPIC || filter == D3D11_FILTER_COMPARISON_ANISOTROPIC)
		{
			sd.MaxAnisotropy = 8;
		}
		else
		{
			sd.MaxAnisotropy = 1u;
		}
		// border color

		if (borderColor)
		{
			memcpy(sd.BorderColor, borderColor, sizeof(float) * 4);
		}
		else
		{
			float greenColor[4] = { 0.0f ,1.0f, 0.0f, 1.0f };
			memcpy(sd.BorderColor, greenColor, sizeof(float) * 4);
		}

		/************** create sampler state ****************/

		GFX_THROW_INFO(pDevice->CreateSamplerState(&sd, &m_pSampler));

	}
	void Sampler::Bind(ID3D11DeviceContext* pContext, UINT slot)
	{
		pContext->PSSetSamplers(slot, 1u, m_pSampler.GetAddressOf());
	}

	void Sampler::BindVS(ID3D11DeviceContext* pContext, UINT slot)
	{
		pContext->VSSetSamplers(slot, 1u, m_pSampler.GetAddressOf());
	}
}