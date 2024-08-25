#pragma once
#include "Shader.h"

namespace yoi
{
	class P2_T2_2D : public Shader
	{
	public:
		P2_T2_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		~P2_T2_2D() = default;
		P2_T2_2D& operator = (const P2_T2_2D&) = delete;
		P2_T2_2D(const P2_T2_2D&) = delete;

		const char* GetVertexShaderPath() const override;
		const char* GetPixelShaderPath() const override;
		const char* GetInputLayoutInfo() const override;
		const char* GetTitle() const override;
	};
}