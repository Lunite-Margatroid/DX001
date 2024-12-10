#include "pch.h"
#include "TypeInfo.h"

namespace yoi
{
	TypeInfo::TypeInfo(ClassType classType, const char* className, unsigned int strLen)
		:m_ClassType(classType), m_ClassName(className, strLen)
	{
	}
}
