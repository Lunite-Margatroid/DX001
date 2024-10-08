#include "pch.h"
#include "SpriteV3.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
namespace yoi
{
	SpriteV3::SpriteV3()
	{
	}
	SpriteV3::SpriteV3(SpriteV3* sprite)
	{
		m_Meshes = sprite->m_Meshes;
	}
	SpriteV3::SpriteV3(const Mesh& mesh)
	{
		AddMesh(mesh);
	}
	SpriteV3::SpriteV3(Mesh&& mesh)
	{
		AddMesh(std::move(mesh));
	}
	SpriteV3::SpriteV3(std::vector<Mesh>& meshes)
	{
		AddMeshes(meshes);
	}
	SpriteV3::SpriteV3(std::vector<Mesh>&& meshes)
	{
		AddMeshes(meshes);
	}
	void SpriteV3::AddMesh(const Mesh& mesh)
	{
		m_Meshes.push_back(mesh);
	}
	void SpriteV3::AddMesh(Mesh&& mesh)
	{
		m_Meshes.emplace_back(std::move(mesh));
	}
	void SpriteV3::AddMeshes(std::vector<Mesh>& meshes)
	{
		std::copy(meshes.begin(), meshes.end(), m_Meshes.end());
	}
	void SpriteV3::AddMeshes(std::vector<Mesh>&& meshes)
	{
		if (m_Meshes.empty())
		{
			m_Meshes = std::move(meshes);
		}
		else
		{
			std::copy(meshes.begin(), meshes.end(), m_Meshes.end());
		}
	}
	void SpriteV3::Render(const glm::mat4& modelMat)
	{
	}
	void SpriteV3::RenderV3(const glm::mat4& modelMat, const CameraObj* camera)
	{
		if (!m_Meshes.empty())
		{
			GFX_EXCEPT_SUPPORT();
			ConstBuffer* constantBuffer = Graphics::GetInstance().GetConstBuffer(BufferManager::Buffers::Constant_Matrix);
			ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();

			glm::mat4 mvpTrans = camera->GetVPTrans() * modelMat;
			D3D11_MAPPED_SUBRESOURCE dataMap;
			GFX_THROW_INFO(pContext->Map(constantBuffer->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &dataMap));
			// memset(dataMap.pData, 0, 64 * 5);
			// model mat
			memcpy(dataMap.pData, glm::value_ptr(modelMat), 64);

			// view mat
			memcpy((BYTE*)dataMap.pData + 64, glm::value_ptr(camera->GetViewTrans()), 64);

			// projection mat
			memcpy((BYTE*)dataMap.pData + 128, glm::value_ptr(camera->GetProjectionTrans()), 64);

			// set MVP Matrix
			memcpy((BYTE*)dataMap.pData + 192, glm::value_ptr(mvpTrans), 64);

			glm::mat4 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMat)));
			// normal mat
			memcpy((BYTE*)dataMap.pData + 256, glm::value_ptr(normalMat), 64);

			// camera pos
			memcpy((BYTE*)dataMap.pData + 320, glm::value_ptr(camera->GetPosition()), 12);

			GFX_THROW_INFO_ONLY(pContext->Unmap(constantBuffer->GetBuffer(), 0));

			constantBuffer->BindVS(pContext, 0u);

			for (Mesh& mesh : m_Meshes)
			{
				mesh.Draw(pContext);
			}
		}
	}
}