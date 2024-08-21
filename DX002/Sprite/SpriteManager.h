#pragma once
#include "SpriteV3.h"
#include "Mesh.h"

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

		/// <summary>
		/// Copy spirte and add new sprite to container.
		/// 'Sprite' will also be added if container didn't contain 'sprite'.
		/// </summary>
		/// <param name="sprite"> </param>
		/// <returns></returns>
		SpriteV3* Copy(SpriteV3* sprite);

	};
}