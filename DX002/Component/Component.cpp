#include "pch.h"
#include "Component.h"

namespace yoi
{
	/************************** Class Component ******************************/
	Component::Component():
		m_pAttachedObj(nullptr)
	{
		m_ComponentType = Type::PureComponent;
	}
	void* Component::GetAttachedObj()
	{
		return m_pAttachedObj;
	}
	void Component::SetAttach(void* attachObj)
	{
		m_pAttachedObj = attachObj;
	}

	/*************************** Class Information ************************************/
	const char* ClassName_Component = "Component";
	Component::ComponentInfo::ComponentInfo()
		:TypeInfo(ClassType::Component, ClassName_Component, strlen(ClassName_Component))
	{}
	Component::ComponentInfo::ComponentInfo(const char* className, unsigned int strLen)
		:TypeInfo(ClassType::Component, className, strLen)
	{}
}