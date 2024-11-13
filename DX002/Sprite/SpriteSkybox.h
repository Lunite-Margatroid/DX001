#pragma once
#include "SpriteV3.h"
#include "Texture/CubeTexture.h"

namespace yoi
{
	class SpriteSkybox : public SpriteV3
	{
	public:
		SpriteSkybox(CubeTexture* tex, Shader* shader);
		~SpriteSkybox() = default;
		SpriteSkybox(const SpriteSkybox&) = delete;
		SpriteSkybox& operator = (const SpriteSkybox&) = delete;
	};
}