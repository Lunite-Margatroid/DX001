#pragma once
#include "SceneObj.h"
#include "Light/Light.h"

namespace yoi
{
	class LightObj : public SceneObj
	{
	protected:
		Light* m_Light;
	public:
		LightObj(SceneObj* parent = nullptr, Sprite* sprite = nullptr, const std::string& objName = "No Name", Light* light = nullptr);
		~LightObj();

		Light* GetAttachedLight();
		void SetAttachedLight(Light* light);

		void Update(float deltaTime) override;
	};
}