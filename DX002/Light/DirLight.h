#pragma once
#include "Light.h"

namespace yoi
{
	class DirLight : virtual public Light
	{
	protected:
		glm::vec3 m_Direction;
	public:
		DirLight(const glm::vec3& color = glm::vec3(1.0f), \
			float ambient = 0.1f, float diffuse = 0.7f, float specular = 0.7f, \
			const glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f));
		DirLight(const DirLight&) = default;
		DirLight& operator = (const DirLight&) = default;
		virtual ~DirLight() = default;

		virtual size_t WriteToBuffer(void* dest, size_t offset) override;
	};
}