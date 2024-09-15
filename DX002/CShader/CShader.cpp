#include "pch.h"
#include "CShader.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	CShader::CShader(ID3D11Device* pDevice, const std::string& path)
	{
		GFX_EXCEPT_SUPPORT();

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		// create compute shader
		GFX_THROW_INFO(D3DReadFileToBlob(UTF8ToWStr(path).c_str(), &pBlob));

		GFX_THROW_INFO(pDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pComputeShader));

	}
	void CShader::Bind(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();

		GFX_THROW_INFO_ONLY(pContext->CSSetShader(m_pComputeShader.Get(), nullptr, 0));
	}
}