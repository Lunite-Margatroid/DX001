#include "pch.h"
#include "Camera2DObj.h"

namespace yoi
{
	void Camera2DObj::UpdateProjectionTrans()
	{
		m_ProjectionTrans = glm::ortho(m_ViewLeft, m_ViewRight, m_ViewBottom, m_ViewTop, m_ViewNear, m_ViewFar);
	}
	Camera2DObj::Camera2DObj(SceneObj* parent, Sprite* sprite, const std::string& objName)
		:CameraObj(parent, sprite, objName),
		m_ViewLeft(-8.0f),
		m_ViewRight(8.0f),
		m_ViewTop(4.5f),
		m_ViewBottom(-4.5f),
		m_ViewFar(-10.0f),
		m_ViewNear(10.0f)
	{
	}
	float Camera2DObj::GetViewLeft() const
	{
		return m_ViewLeft;
	}
	float Camera2DObj::GetViewRight() const
	{
		return m_ViewRight;
	}
	float Camera2DObj::GetViewTop() const
	{
		return m_ViewTop;
	}
	float Camera2DObj::GetViewBottom() const
	{
		return m_ViewBottom;
	}
	float Camera2DObj::GetViewNear() const
	{
		return m_ViewNear;
	}
	float Camera2DObj::GetViewFar() const
	{
		return m_ViewFar;
	}
	void Camera2DObj::SetViewLeft(float left)
	{
		m_ViewLeft = left;
	}
	void Camera2DObj::SetViewRight(float right)
	{
		m_ViewRight = right;
	}
	void Camera2DObj::SetViewTop(float top)
	{
		m_ViewTop = top;
	}
	void Camera2DObj::SetViewBottom(float bottom)
	{
		m_ViewBottom = bottom;
	}
	void Camera2DObj::SetViewNear(float zNear)
	{
		m_ViewNear = zNear;
	}
	void Camera2DObj::SetViewFar(float zFar)
	{
		m_ViewFar = zFar;
	}
	void Camera2DObj::SetViewWidth(float width)
	{
		m_ViewRight = width / 2.0f;
		m_ViewLeft = -m_ViewRight;
	}
	void Camera2DObj::SetViewHeight(float height)
	{
		m_ViewTop = height / 2.0f;
		m_ViewBottom = -m_ViewTop;
	}
	void Camera2DObj::HeadTo(const glm::vec3& target)
	{
		m_Position.x = target.x;
		m_Position.y = target.y;
	}
}