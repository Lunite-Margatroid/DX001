#include "pch.h"
#include "P2_T2_2D.h"

namespace yoi
{
	P2_T2_2D::P2_T2_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		D3D11_INPUT_ELEMENT_DESC ed[] =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		Init(pDevice, pContext, ed, 2);
	}
	const char* P2_T2_2D::GetVertexShaderPath() const
	{
		return "shader-bin/Textured2DVS.cso";
	}
	const char* P2_T2_2D::GetPixelShaderPath() const
	{
		return "shader-bin/Textured2DPS.cso";
	}
	const char* P2_T2_2D::GetInputLayoutInfo() const
	{
		return "Position: float32 * 2\nTexCoord: float32 * 2";
	}
	const char* P2_T2_2D::GetTitle() const
	{
		return "Textured Shader 2D";
	}
}