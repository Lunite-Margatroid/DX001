#pragma once

#include "Shader.h"

namespace yoi
{
	class P3_N3_T2_ClothSim : public Shader
	{
	public:
		P3_N3_T2_ClothSim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		{
			D3D11_INPUT_ELEMENT_DESC ed[] =
			{
				{"Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"Normal",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT,       2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			Init(pDevice, pContext, ed, 3);
		}
		P3_N3_T2_ClothSim(const P3_N3_T2_ClothSim&) = delete;
		P3_N3_T2_ClothSim& operator = (const P3_N3_T2_ClothSim&) = delete;
		~P3_N3_T2_ClothSim() = default;


		virtual const char* GetVertexShaderPath() const override
		{
			return "shader-bin/ClothSimVS.cso";
		}
		virtual const char* GetPixelShaderPath() const override
		{
			return "shader-bin/LightedMaterialedPS.cso";
		}
		virtual const char* GetInputLayoutInfo() const override
		{
			return "Position: float32 * 3\nTexCoord: float32 * 2";
		}
		virtual const char* GetTitle() const override
		{
			return "Cloth Simulation Shader";
		}
	};
}
