#include "pch.h"
#include "SpriteManager.h"

namespace yoi
{
	SpriteManager::~SpriteManager()
	{
		for (SpriteV3* sprite : m_Sprites)
			delete sprite;
	}

	SpriteV3* SpriteManager::Copy(SpriteV3* sprite)
	{
		if (m_Sprites.find(sprite) == m_Sprites.end())
		{// didn't contain
			m_Sprites.insert(sprite);
		}
		SpriteV3* newSprite = new SpriteV3(sprite);
		m_Sprites.insert(newSprite);
		return newSprite;
	}
}
