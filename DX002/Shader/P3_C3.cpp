#include "pch.h"
#include "P3_C3.h"

namespace yoi
{

	P3_C3::P3_C3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		D3D11_INPUT_ELEMENT_DESC ed[] = 
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		Init(pDevice, pContext,ed, 2);
	}
	const char* P3_C3::GetVertexShaderPath() const
	{
		return "shader-bin/VertexShader.cso";
	}
	const char* P3_C3::GetPixelShaderPath() const
	{
		return "shader-bin/PixelShader.cso";
	}
	const char* P3_C3::GetInputLayoutInfo() const
	{
		return "Position: float32 * 3\nColor:    BYTE * 4";
	}
	const char* P3_C3::GetTitle() const
	{
		return "Colored Vertex Shader";
	}
}
