#pragma once
#include "Light.h"

namespace yoi
{
	class PointLight : virtual public Light
	{
	protected:
		glm::vec3 m_Position;
		float m_kConstant;
		float m_kLinear;
		float m_kQuadratic;
	public:
		PointLight(const glm::vec3& color = glm::vec3(1.0f), \
			float ambient = 0.1f, float diffuse = 0.7f, float specular = 0.7f, \
			const glm::vec3& position = glm::vec3(0.0f), \
			float kConstant = 1.0f, float kLinear = 0.14f, float kQuadratic = 0.07f,\
			LightObj* obj = nullptr);
		PointLight(const PointLight&) = default;
		PointLight& operator = (const PointLight&) = default;
		virtual ~PointLight() = default;

		virtual size_t WriteToBuffer(void* dest, size_t offset) override;

		void UpdatePosition() override;
	};
}