#pragma once
#include "Shader.h"

namespace yoi
{
	class P3_T2 : public Shader
	{
	public:
		P3_T2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		~P3_T2() = default;
		P3_T2(const P3_T2&) = delete;
		P3_T2& operator = (const P3_T2&) = delete;

		const char* GetVertexShaderPath() const override;
		const char* GetPixelShaderPath() const override;
		const char* GetInputLayoutInfo() const override;
		const char* GetTitle() const override;
	};
}