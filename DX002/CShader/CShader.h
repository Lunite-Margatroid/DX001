#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <wchar.h>
namespace yoi
{
	class CShader
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_pComputeShader;

	public:
		CShader(ID3D11Device* pDevice, const std::string& path);
		~CShader() = default;
		CShader(const CShader& shader) = delete;
		CShader& operator = (const CShader& shader) = delete;

		void Bind(ID3D11DeviceContext* pContext);
	};
}