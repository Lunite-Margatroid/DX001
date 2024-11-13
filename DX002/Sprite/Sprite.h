#pragma once
#include <d3d11.h>

namespace yoi
{
	class Sprite
	{
	public:
		enum class SpriteType
		{
			Untransparent,
			Transparent,
			Skybox
		};
	public:
		SpriteType m_Type;

		Sprite();
		virtual ~Sprite() = default;

		virtual void Update(float deltaTime);
		virtual void Render(const glm::mat4& modelMat) = 0;
		virtual void RenderImGui();
	};
}