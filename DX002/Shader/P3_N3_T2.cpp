#include "pch.h"
#include "P3_N3_T2.h"

namespace yoi
{
	P3_N3_T2::P3_N3_T2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		D3D11_INPUT_ELEMENT_DESC ed[] =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		Init(pDevice, pContext, ed, 3);
	}

	const char* P3_N3_T2::GetVertexShaderPath() const
	{
		return "shader-bin/TexColorVS.cso";
	}
	const char* P3_N3_T2::GetPixelShaderPath() const
	{
		return "shader-bin/TexColorPS.cso";
	}
	const char* P3_N3_T2::GetInputLayoutInfo() const
	{
		return "Position: float32 * 3\nNormal: float32 * 3\nTexCoord: float32 * 2";
	}
	const char* P3_N3_T2::GetTitle() const
	{
		return "Texture Color Shader";
	}

}
