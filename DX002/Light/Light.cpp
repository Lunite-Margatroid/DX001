#include "pch.h"
#include "Light.h"
namespace yoi
{
	Light::Light(const glm::vec3& color, float ambient, float diffuse, float specular)
		:m_Color(color)
	{
		m_Contribution[0] = ambient;
		m_Contribution[1] = diffuse;
		m_Contribution[2] = specular;
	}
}