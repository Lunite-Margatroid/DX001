#include "pch.h"
#include "Sampler.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	Sampler::Sampler()
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();


		D3D11_SAMPLER_DESC sd = {};
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		GFX_THROW_INFO(pDevice->CreateSamplerState(&sd, &m_pSampler));

	}
	void Sampler::Bind(ID3D11DeviceContext* pContext)
	{
		pContext->PSSetSamplers(0u, 1u, m_pSampler.GetAddressOf());
	}
}