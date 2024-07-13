#pragma once
#include "Sprite.h"
#include <d3d11.h>
#include <wrl.h>
#include "Shader\Shader.h"

namespace yoi
{
	class SpriteV1 : public Sprite
	{
	protected:
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContext;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;

		Shader* m_Shader;

		SpriteV1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	public:
		SpriteV1() = delete;
		virtual ~SpriteV1() = default;
		virtual void Render(const glm::mat4& modelMat);
		virtual void RenderV1(const glm::mat4& modelMat, const glm::mat4& vpTrans) = 0;
	};
}