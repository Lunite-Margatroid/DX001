#pragma once
#include <d3d11.h>

namespace yoi
{
	class Sprite
	{
	protected:

	public:
		Sprite() = default;
		virtual ~Sprite() = default;

		virtual void Update(float deltaTime);
		virtual void Render(const glm::mat4& modelMat) = 0;
		virtual void RenderImGui();
	};
}