#include "pch.h"
#include "P3_T2_Materialed.h"
namespace yoi
{
	yoi::P3_T2_Materialed::P3_T2_Materialed(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		D3D11_INPUT_ELEMENT_DESC ed[] =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		Init(pDevice, pContext, ed, 2);
	}
	const char* P3_T2_Materialed::GetVertexShaderPath() const
	{
		return "shader-bin/MaterialedVS.cso";
	}
	const char* P3_T2_Materialed::GetPixelShaderPath() const
	{
		return "shader-bin/MaterialedPS.cso";
	}
	const char* P3_T2_Materialed::GetInputLayoutInfo() const
	{
		return "Position: float32 * 3\nTexCoord: float32 * 2";
	}
	const char* P3_T2_Materialed::GetTitle() const
	{
		return "Materialed Shader";
	}
}