#pragma once
#include "Sprite.h"
#include "BufferManager\BufferManager.h"
#include "Mesh.h"
#include "Shader\Shader.h"

namespace yoi
{
	class SpriteV2 : public Sprite
	{
	protected:
		std::vector<Mesh> m_Meshes;
	public:
		SpriteV2();
		~SpriteV2() = default;
		SpriteV2(const SpriteV2&) = delete;
		SpriteV2& operator = (const SpriteV2&) = delete;

		void AddMesh(Mesh& mesh);
		void AddMesh(Mesh&& mesh);

		virtual void Render(const glm::mat4& modelMat) override;
		virtual void RenderV2(const glm::mat4& modelMat, const glm::mat4& vpTrans);
	};
}