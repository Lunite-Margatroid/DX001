#include "pch.h"
#include "SpriteSkybox.h"
#include "Renderer\Graphics.h"

namespace yoi
{
	SpriteSkybox::SpriteSkybox(CubeTexture* tex, Shader* shader)
	{
		m_Type = SpriteType::Skybox;

		Material* material = Graphics::GetInstance().GetMaterialManager()->CreateMaterial(
			dynamic_cast<Texture*>(tex),
			Graphics::GetInstance().GetTextureManager()
		);
		
		Mesh mesh(
			Graphics::GetVertexBuffer(BufferManager::Buffers::Vertex_P3_Skybox),
			Graphics::GetIndexBuffer(BufferManager::Buffers::Index_Skybox),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			shader,
			material
		);

		m_Meshes.emplace_back(std::move(mesh));
	}
}