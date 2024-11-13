#include "Shader.h"

namespace yoi
{
	class P3_Skybox : public Shader
	{
	public:
		P3_Skybox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		{
			D3D11_INPUT_ELEMENT_DESC ed[] =
			{
				{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			Init(pDevice, pContext, ed, 1);
		}

		P3_Skybox(const P3_Skybox&) = delete;
		P3_Skybox& operator = (const P3_Skybox&) = delete;
		~P3_Skybox() = default;

		virtual const char* GetVertexShaderPath() const override
		{
			return "shader-bin/SkyboxVS.cso";
		}

		virtual const char* GetPixelShaderPath() const override
		{
			return "shader-bin/SkyboxPS.cso";
		}

		virtual const char* GetInputLayoutInfo() const override
		{
			return "Position: float32 * 3";
		}

		virtual const char* GetTitle() const override
		{
			return "Skybox Shader";
		}
	};
}