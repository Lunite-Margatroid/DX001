#include "pch.h"
#include "Shader.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	void Shader::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LPCWSTR vsFilePath, LPCWSTR psFilePath, D3D11_INPUT_ELEMENT_DESC* attribs, UINT numAttrib)
	{
		GFX_EXCEPT_SUPPORT();


		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		// create pixel shader
		GFX_THROW_INFO(D3DReadFileToBlob(psFilePath, &pBlob));
		GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));

		// create vertex shader
		GFX_THROW_INFO(D3DReadFileToBlob(vsFilePath, &pBlob));
		GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVertexShader));

		GFX_THROW_INFO(pDevice->CreateInputLayout(attribs, numAttrib, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pLayout));
	}
	void Shader::Bind(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->VSSetShader(m_pVertexShader.Get(), 0, 0u));
		GFX_THROW_INFO_ONLY(pContext->PSSetShader(m_pPixelShader.Get(), 0, 0u));
		GFX_THROW_INFO_ONLY(pContext->IASetInputLayout(m_pLayout.Get()));
	}
	const char* Shader::GetShaderDescript() const
	{
		std::stringstream oss;
		oss << "[Shader Info]" << std::endl
			<< "Title: " << GetTitle() << std::endl
			<< "Vertex shader path: " << GetVertexShaderPath() << std::endl
			<< "Pixel shader path:  " << GetPixelShaderPath() << std::endl
			<< GetInputLayoutInfo() << std::endl;
		std::string strBuffer = std::move(oss.str());
		return strBuffer.c_str();
	}
}