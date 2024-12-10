#pragma once
#include "Component.h"

namespace yoi
{
	class ComponentRect : public Component
	{
	public:
		class RectInfo : public ComponentInfo 
		{
		public:
			RectInfo();
			~RectInfo() = default;
			RectInfo& operator = (const RectInfo&) = delete;
			RectInfo(const RectInfo&) = delete;
		};
	protected:
		glm::vec2 m_RelativePos;
		float m_Width;
		float m_Height;
	public:
		ComponentRect();
		ComponentRect(float relativeX, float relativeY, float width, float height);
		ComponentRect(const glm::vec2& relativePos, float width, float height);
		~ComponentRect() = default;
		ComponentRect(const Component&) = delete;
		ComponentRect& operator = (const ComponentRect&) = delete;

		float GetRelativeX() const;
		float GetRelativeY() const;
		glm::vec2 GetRelativePos() const;
		float GetWidth() const;
		float GetHeight() const;

		void SetRelativeX(float relativeX);
		void SetRelativeY(float relativeY);
		void SetRelativePos(const glm::vec2 relativePos);
		void SetWidth(float width);
		void SetHeight(float height);
	};
}