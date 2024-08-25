#pragma once
#include "CameraObj.h"

namespace yoi
{
	class Camera2DObj : public CameraObj
	{
	protected:
		float m_ViewLeft;
		float m_ViewRight;
		float m_ViewTop;
		float m_ViewBottom;
		float m_ViewNear;
		float m_ViewFar;

		virtual void UpdateProjectionTrans() override;
	public:
		Camera2DObj(SceneObj* parent = nullptr, Sprite* sprite = nullptr, const std::string& objName = "Camera 2D");

		float GetViewLeft() const;
		float GetViewRight() const;
		float GetViewTop() const;
		float GetViewBottom() const;
		float GetViewNear() const;
		float GetViewFar() const;

		void SetViewLeft(float);
		void SetViewRight(float);
		void SetViewTop(float);
		void SetViewBottom(float);
		void SetViewNear(float);
		void SetViewFar(float);

		void SetViewWidth(float width);
		void SetViewHeight(float height);

		void HeadTo(const glm::vec3& target) override;
	};
}