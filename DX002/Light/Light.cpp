#include "pch.h"
#include "Light.h"
#include "SceneObject\LightObj.h"
namespace yoi
{
	Light::Light(const glm::vec3& color, float ambient, float diffuse, float specular, LightObj* obj)
		:m_Color(color), m_AttachObj(obj)
	{
		m_Contribution[0] = ambient;
		m_Contribution[1] = diffuse;
		m_Contribution[2] = specular;
	}
	Light::~Light()
	{
		if (m_AttachObj)
		{
			m_AttachObj->SetAttachedLight(nullptr);
		}
	}
	void Light::SetAttachedObj(LightObj* obj)
	{
		m_AttachObj = obj;
	}
	LightObj* Light::GetAttachedObj()
	{
		return m_AttachObj;
	}
	void Light::SetLightColor(const glm::vec3& color)
	{
		m_Color = color;
	}
	void Light::SetContribution(float ambient, float diffuse, float specular)
	{
		m_Contribution[0] = ambient;
		m_Contribution[1] = diffuse;
		m_Contribution[2] = specular;
	}
}