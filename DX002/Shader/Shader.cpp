#include "pch.h"
#include "Shader.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	void Shader::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, D3D11_INPUT_ELEMENT_DESC* attribs, UINT numAttrib)
	{
		GFX_EXCEPT_SUPPORT();


		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		// create pixel shader
		GFX_THROW_INFO(D3DReadFileToBlob(UTF8ToWStr(std::string(GetPixelShaderPath())).c_str(), &pBlob));
		GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));

		// create vertex shader
		GFX_THROW_INFO(D3DReadFileToBlob(UTF8ToWStr(std::string(GetVertexShaderPath())).c_str(), &pBlob));
		GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVertexShader));

		GFX_THROW_INFO(pDevice->CreateInputLayout(attribs, numAttrib, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pLayout));

		FileLogger::Info(GetShaderDescript().c_str());
	}
	void Shader::Bind(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->VSSetShader(m_pVertexShader.Get(), 0, 0u));
		GFX_THROW_INFO_ONLY(pContext->PSSetShader(m_pPixelShader.Get(), 0, 0u));
		GFX_THROW_INFO_ONLY(pContext->IASetInputLayout(m_pLayout.Get()));
	}
	std::string Shader::GetShaderDescript() const
	{
		std::stringstream oss;
		oss << "[Shader Info]" << std::endl
			<< "Title: " << GetTitle() << std::endl
			<< "Vertex shader path: " << GetVertexShaderPath() << std::endl
			<< "Pixel shader path:  " << GetPixelShaderPath() << std::endl
			<< GetInputLayoutInfo() << std::endl;
		std::string strBuffer = std::move(oss.str());
		return strBuffer;
	}

	std::wstring UTF8ToWStr(const std::string& str)
	{
		std::wstring wstr;

		int len = static_cast<int>(str.size());
		int nConvent = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(),  len,  nullptr, 0);
		if (nConvent <= 0)
		{
			FileLogger::Error("Code Converting Failed.");
			return std::wstring(L"Code Converting Failed.");
		}
		else
		{
			wstr.resize(nConvent);
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), len, &wstr[0], (int)wstr.size());
		}
		return wstr;
	}
}