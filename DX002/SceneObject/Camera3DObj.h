#pragma once
#include "CameraObj.h"

namespace yoi
{
	class Camera3DObj : public CameraObj
	{
	protected:
		float m_ViewRangeScale;

		float m_SenRad;				// Sensitivity Of rotate
		float m_SenPos;				// Sensitivity Of Move
		float m_SenViewRange;		// Sensitivity of view range change

		float m_Near;
		float m_Far;


	public:
		Camera3DObj(SceneObj* parent = nullptr, Sprite* sprite = nullptr, const std::string& objName = "No Name");

		void Move(Direction dir, float len);
		void RotateYaw(float yaw);
		void RotateRoll(float roll);
		void RotatePitch(float pitch);
		void ViewRangeChange(float delta);

		void Update(float deltaTime) override;

		void SetNear(float zNear);
		void SetFar(float zFar);

		float GetNear() const;
		float GetFar() const;

		void RenderImGui() override;

		virtual void HeadTo(const glm::vec3& target) override;
	};
}