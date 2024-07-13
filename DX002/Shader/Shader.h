#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

namespace yoi
{
	class Shader
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pLayout;

		void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LPCWSTR vsFilePath, LPCWSTR psFilePath, D3D11_INPUT_ELEMENT_DESC* attribs, UINT numAttrib);
	public:
		virtual void Bind(ID3D11DeviceContext* pContext);
		virtual const char* GetVertexShaderPath() const = 0;
		virtual const char* GetPixelShaderPath() const = 0;
		virtual const char* GetShaderDescript() const;
		virtual const char* GetInputLayoutInfo() const = 0;
		virtual const char* GetTitle() const = 0;
	};
}