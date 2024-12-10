#include "pch.h"
#include "ComponentRect.h"

namespace yoi
{
	/******************************** Class Component ***********************************/
	ComponentRect::ComponentRect()
		:m_RelativePos(0.f, 0.f), m_Width(0.0f), m_Height(0.0f)
	{
	}
	ComponentRect::ComponentRect(float relativeX, float relativeY, float width, float height)
		:m_RelativePos(relativeX, relativeY), m_Width(width), m_Height(height)
	{
	}
	ComponentRect::ComponentRect(const glm::vec2& relativePos, float width, float height)
		:m_RelativePos(relativePos), m_Width(width), m_Height(height)
	{
	}
	float ComponentRect::GetRelativeX() const
	{
		return m_RelativePos.x;
	}
	float ComponentRect::GetRelativeY() const
	{
		return m_RelativePos.y;
	}
	glm::vec2 ComponentRect::GetRelativePos() const
	{
		return m_RelativePos;
	}
	float ComponentRect::GetWidth() const
	{
		return m_Width;
	}
	float ComponentRect::GetHeight() const
	{
		return m_Height;
	}
	void ComponentRect::SetRelativeX(float relativeX)
	{
		m_RelativePos.x = relativeX;
	}
	void ComponentRect::SetRelativeY(float relativeY)
	{
		m_RelativePos.y = relativeY;
	}
	void ComponentRect::SetRelativePos(const glm::vec2 relativePos)
	{
		m_RelativePos = relativePos;
	}
	void ComponentRect::SetWidth(float width)
	{
		m_Width = width;
	}
	void ComponentRect::SetHeight(float height)
	{
		m_Height = height;
	}

	/*********************** Class Information ********************************/
	const char* ClassName_ComponentRect = "ComponentRect";
	ComponentRect::RectInfo::RectInfo()
		:ComponentInfo(ClassName_ComponentRect, strlen(ClassName_ComponentRect))
	{
		ComponentRect rectObject;

		const char* properity_1 = "RelativePos";
		m_Properities.emplace_back(
			MEMBER_OFFSET(&rectObject, &rectObject.m_RelativePos),
			ComponentProperityType::GlmVec2,
			std::move(std::string_view(properity_1, strlen(properity_1))));

		const char* properity_2 = "Width";
		m_Properities.emplace_back(
			MEMBER_OFFSET(&rectObject, &rectObject.m_Width),
			ComponentProperityType::Float32,
			std::move(std::string_view(properity_2, strlen(properity_2))));

		const char* properity_3 = "Height";
		m_Properities.emplace_back(
			MEMBER_OFFSET(&rectObject, &rectObject.m_Height),
			ComponentProperityType::Float32,
			std::move(std::string_view(properity_3, strlen(properity_3))));
	}
}