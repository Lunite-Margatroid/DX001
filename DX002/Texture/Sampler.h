#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace yoi
{
	class Sampler
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
	public:
		/// <summary>
		/// Create a Sampler State
		/// </summary>
		/// <param name="filter"> the filter method. The Anisotropy will be set 8 if Anisotropy Filter is set.</param>
		/// <param name="wrap"> the method of wrap </param>
		/// <param name="borderColor"> the border. a vector of float32 with 4 element. default [0,1,0,1] </param>
		Sampler(D3D11_FILTER filter  = D3D11_FILTER_MIN_MAG_MIP_POINT,
			D3D11_TEXTURE_ADDRESS_MODE wrap = D3D11_TEXTURE_ADDRESS_WRAP,
			float* borderColor = nullptr);

		~Sampler() = default;
		Sampler(const Sampler&) = delete;
		Sampler& operator = (const Sampler&) = delete;
		void Bind(ID3D11DeviceContext* pContext, UINT slot = 0);
	};
}