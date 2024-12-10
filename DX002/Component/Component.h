#pragma once
#include "Reflection/TypeInfo.h"

namespace yoi
{
	class Component
	{
	public:
		class ComponentInfo : public TypeInfo
		{
		protected:
			std::vector<ComponentProperity> m_Properities;
		public:
			ComponentInfo();
			ComponentInfo(const char* className, unsigned int strLen);
			ComponentInfo(const Component&) = delete;
			ComponentInfo& operator = (const Component&) = delete;
			~ComponentInfo() = default;
		};

	public:
		enum class Type
		{
			PureComponent, Rect, CollisionBox2D
		};

	protected:
		void* m_pAttachedObj;
		Type m_ComponentType;
	public:
		Component();
		Component(const Component&) = delete;
		virtual ~Component() = default;
		Component& operator = (const Component& cmpt) = delete;

		void* GetAttachedObj();
		void SetAttach(void* attachObj);

		virtual void Update(float deltaTime);
	};
}