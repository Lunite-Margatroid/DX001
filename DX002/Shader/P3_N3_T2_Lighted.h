#pragma once
#include "P3_T2_Materialed.h"

namespace yoi
{
	class P3_N3_T2_Lighted : public Shader
	{
	public:
		P3_N3_T2_Lighted(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		P3_N3_T2_Lighted(const P3_N3_T2_Lighted&) = delete;
		P3_N3_T2_Lighted& operator = (const P3_N3_T2_Lighted&) = delete;
		~P3_N3_T2_Lighted() = default;

		const char* GetVertexShaderPath() const override;
		const char* GetPixelShaderPath() const override;
		const char* GetInputLayoutInfo() const override;
		const char* GetTitle() const override;
	};
}