#pragma once
#include "Sprite.h"
#include "BufferManager\BufferManager.h"
#include "Mesh.h"
#include "Shader\Shader.h"

namespace yoi
{
	class CameraObj;

	class SpriteV3 : public Sprite
	{
	protected:
		std::vector<Mesh> m_Meshes;
	public:
		SpriteV3();
		~SpriteV3() = default;
		SpriteV3(const SpriteV3&) = delete;
		SpriteV3& operator = (const SpriteV3&) = delete;

		void AddMesh(Mesh& mesh);
		void AddMesh(Mesh&& mesh);

		virtual void Render(const glm::mat4& modelMat) override;
		virtual void RenderV3(const glm::mat4& modelMat, const CameraObj* camera);
	};
}