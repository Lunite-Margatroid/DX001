#pragma once
#include "SpriteV3.h"
#include "WaterWave.h"
#include "Mesh.h"
#include "BufferManager\BufferManager.h"
#include "Shader\ShaderManager.h"

namespace yoi
{
	class SpriteManager
	{
	private:
		std::set<SpriteV3*> m_Sprites;
	public:
		SpriteManager() = default;
		~SpriteManager();

		SpriteManager(const SpriteManager&) = delete;
		SpriteManager& operator = (const SpriteManager&) = delete;

		template<typename ... Args>
		SpriteV3* Sprite(Args...args)
		{
			SpriteV3* sprite = new SpriteV3(args...);
			m_Sprites.insert(sprite);
			return sprite;
		}

		/****************** yoi::SpriteManager::Copy ******************/
		/// <summary>
		/// Copy spirte and add new sprite to container.
		/// 'Sprite' will also be added if container didn't contain 'sprite'.
		/// </summary>
		/// <param name="sprite"> </param>
		/// <returns></returns>
		SpriteV3* Copy(SpriteV3* sprite);

		/*************** yoi::SpriteManager::CreateGrid ****************/
		/// <summary>
		/// Gen a Grid sprite. The actuall size of the grid will be (raw - 1) x (col - 1).
		/// </summary>
		/// <param name="raw"> the vertex num of the raw of the grid </param>
		/// <param name="col"> the vertex num of the column of the grid </param>
		/// <param name="shader"> the shader for the sprite </param>
		/// <param name="material"> the material for the sprite </param>
		/// <param name="bufferManager"> the buffer manager of current rendrerer </param>
		/// <param name="GenHeight"> the height-gen function for the grid </param>
		/// <param name="GenNormal"> the normal-gen function for the grid </param>
		/// <returns></returns>
		SpriteV3* CreateGrid(int raw, int col, Shader* shader, Material* material,
			BufferManager& bufferManager,
			const std::function<float(float, float)>& GenHeight = [](float x, float y) { return 0.0f; },
			const std::function<glm::vec3(float, float)>& GenNormal = [](float x, float y) { return glm::vec3(0.0f, 1.0f, 0.0f); });


		WaterWave* CreateWave(int width, 
			ShaderManager&  shaderManager, 
			BufferManager& bufferManager,
			TextureManager& texManager,
			MaterialManager& materialManager,
			Texture* diffuseTexture = nullptr,
			Texture* specularTexture = nullptr,
			const std::function<float(float, float)>& GenHeight = [](float x, float y) { return 0.0f; },
			const std::function<glm::vec3(float, float)>& GenNormal = [](float x, float y) { return glm::vec3(0.0f, 1.0f, 0.0f); });
	};
}