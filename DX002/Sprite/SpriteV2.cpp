#include "pch.h"
#include "SpriteV2.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
namespace yoi
{
	SpriteV2::SpriteV2()
	{
	}
	void SpriteV2::AddMesh(Mesh& mesh)
	{
		m_Meshes.push_back(mesh);
	}
	void SpriteV2::AddMesh(Mesh&& mesh)
	{
		m_Meshes.emplace_back(mesh);
	}
	void SpriteV2::Render(const glm::mat4& modelMat)
	{
	}
	void SpriteV2::RenderV2(const glm::mat4& modelMat, const glm::mat4& vpTrans)
	{
		if (!m_Meshes.empty())
		{
			GFX_EXCEPT_SUPPORT();
			ID3D11Buffer* constantBuffer = Graphics::GetInstance().GetBuffer(BufferManager::Buffer::Constant_Matrix);
			ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();

			glm::mat4 mvpTrans = vpTrans * modelMat;
			D3D11_MAPPED_SUBRESOURCE dataMap;
			GFX_THROW_INFO(pContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataMap));
			// memset(dataMap.pData, 0, 64 * 5);
			memcpy((BYTE*)dataMap.pData + 64 * 3, glm::value_ptr(mvpTrans), 64);
			
			GFX_THROW_INFO_ONLY(pContext->Unmap(constantBuffer, 0));

			GFX_THROW_INFO_ONLY(pContext->VSSetConstantBuffers(0, 1, &constantBuffer));
			for (Mesh& mesh : m_Meshes)
			{
				mesh.Draw(pContext);
			}
		}
	}
}