#include "pch.h"
#include "LightObj.h"

namespace yoi
{

	yoi::LightObj::LightObj(SceneObj* parent, Sprite* sprite, const std::string& objName, Light* light)
		:SceneObj(parent, sprite, objName), m_Light(light)
	{
		if (m_Light)
			m_Light->SetAttachedObj(this);
	}
	LightObj::~LightObj()
	{
		if (m_Light)
		{
			m_Light->SetAttachedObj(nullptr);
		}
	}
	Light* LightObj::GetAttachedLight()
	{
		return m_Light;
	}
	void LightObj::SetAttachedLight(Light* light)
	{
		m_Light = light;
	}
	void LightObj::Update(float deltaTime)
	{
		SceneObj::Update(deltaTime);
		if (m_Light)
			m_Light->UpdatePosition();
	}
}
