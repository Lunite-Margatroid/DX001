#pragma once
#include "PointLight.h"
#include "DirLight.h"

namespace yoi
{
	class SpotLight : public PointLight, public DirLight
	{
	protected:
		float m_OuterEdge;
		float m_InnerEdge;
	public:
		SpotLight(const glm::vec3& color = glm::vec3(1.0f), \
			float ambient = 0.1f, float diffuse = 0.7f, float specular = 0.7f, \
			const glm::vec3& position = glm::vec3(0.0f), \
			float kConstant = 1.0f, float kLinear = 0.14f, float kQuadratic = 0.07f,\
			const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),\
			float outerEdge = 0.94f, float innerEdge = 0.96,\
			LightObj* obj = nullptr
		);

		SpotLight(const SpotLight&) = default;
		SpotLight& operator = (const SpotLight&) = default;
		~SpotLight() = default;

		size_t WriteToBuffer(void* dest, size_t offset) override;

		void UpdatePosition() override;

		// input the cos of half view angel 
		void SetOuterEdge(float outer);

		// input the cos of half view angel
		void SetinnerEdge(float inner);
	};
}
