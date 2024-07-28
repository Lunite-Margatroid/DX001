#pragma once
#include "Shader.h"
namespace yoi
{
	class P3_T2_Materialed : public Shader
	{
	protected:
	public:

		P3_T2_Materialed(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		~P3_T2_Materialed() = default;
		P3_T2_Materialed(const P3_T2_Materialed&) = delete;
		P3_T2_Materialed& operator = (const P3_T2_Materialed&) = delete;

		const char* GetVertexShaderPath() const override;
		const char* GetPixelShaderPath() const override;
		const char* GetInputLayoutInfo() const override;
		const char* GetTitle() const override;
	};
}