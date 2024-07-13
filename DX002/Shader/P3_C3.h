#pragma once
#include "Shader.h"

namespace yoi
{
	class P3_C3 : public Shader
	{
	public:
		P3_C3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		~P3_C3() = default;
		P3_C3(const P3_C3&) = delete;
		P3_C3& operator = (const P3_C3&) = delete;

		const char* GetVertexShaderPath() const override;
		const char* GetPixelShaderPath() const override;
		const char* GetInputLayoutInfo() const override;
		const char* GetTitle() const override;
	};
}