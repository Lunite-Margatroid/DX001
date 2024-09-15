#pragma once
#include "Shader.h"

namespace yoi
{
	class P3_N3_T2_Wave : public Shader
	{
	public:
		P3_N3_T2_Wave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		P3_N3_T2_Wave(const P3_N3_T2_Wave&) = delete;
		P3_N3_T2_Wave& operator = (const P3_N3_T2_Wave&) = delete;
		~P3_N3_T2_Wave() = default;

		virtual const char* GetVertexShaderPath() const override;
		virtual const char* GetPixelShaderPath() const override;
		virtual const char* GetInputLayoutInfo() const override;
		virtual const char* GetTitle() const override;
		
	};
}