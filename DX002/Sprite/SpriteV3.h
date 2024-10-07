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
		friend class SpriteManager;
	protected:
		std::vector<Mesh> m_Meshes;

		SpriteV3();
		// copy mesh from sprite
		SpriteV3(SpriteV3* sprite);
		SpriteV3(const Mesh& mesh);
		SpriteV3(Mesh&& mesh);
		SpriteV3(std::vector<Mesh>& meshes);
		SpriteV3(std::vector<Mesh>&& meshes);

	public:
		
		~SpriteV3() = default;
		SpriteV3(const SpriteV3&) = delete;
		SpriteV3& operator = (const SpriteV3&) = delete;

		void AddMesh(const Mesh& mesh);
		void AddMesh(Mesh&& mesh);

		void AddMeshes(std::vector<Mesh>& meshes);
		void AddMeshes(std::vector<Mesh>&& meshes);


		virtual void Render(const glm::mat4& modelMat) override;
		virtual void RenderV3(const glm::mat4& modelMat, const CameraObj* camera);
	};
}