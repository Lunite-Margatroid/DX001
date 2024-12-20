#pragma once
#include "AnimatedObj.h"

namespace yoi
{
	class CameraObj : public AnimatedObj
	{
	public:
		enum class Direction
		{
			front, back, left, right, up, down
		};
		enum class CameraType
		{
			PerspectiveCamera3D, OrthographicCamera3D
		};

	protected:
		mutable glm::mat4 m_ViewTrans;
		mutable glm::mat4 m_ProjectionTrans;
		mutable glm::mat4 m_VPTrans;

		glm::vec3 m_HeadTo;

		glm::vec3 m_Right;
		glm::vec3 m_Front;
		glm::vec3 m_Up;

		void UpdateDirectionVec(const glm::mat3& parentRotation);

		virtual void UpdateViewTrans();
		virtual void UpdateProjectionTrans() = 0 ;
		virtual void UpdateVPTrans();
	public:
		CameraObj(SceneObj* parent = nullptr, Sprite* sprite = nullptr, const std::string& objName = "No Name");

		virtual void Update(float delta) override;

		const glm::mat4& GetViewTrans() const;
		const glm::mat4& GetProjectionTrans() const;
		const glm::mat4& GetVPTrans() const;

		const glm::vec3& GetHeadTo() const;
		virtual void HeadTo(const glm::vec3& target) = 0;

		void RenderImGui() override;

	};
}