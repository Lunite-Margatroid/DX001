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
		Sampler();
		~Sampler() = default;
		Sampler(const Sampler&) = delete;
		Sampler& operator = (const Sampler&) = delete;
		void Bind(ID3D11DeviceContext* pContext);
	};
}