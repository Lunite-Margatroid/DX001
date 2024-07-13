#include "pch.h"
#include "SpriteV1.h"
namespace yoi
{
	SpriteV1::SpriteV1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		:m_pDevice(pDevice), m_pContext(pContext)
	{
	}
	void SpriteV1::Render(const glm::mat4& modelMat)
	{

	}
}