#pragma once
#include "Component.h"

namespace yoi
{
	class ComponentManager
	{
		
	public:
		ComponentManager();
		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator = (const ComponentManager&) = delete;
		virtual ~ComponentManager() = default;
		static ComponentManager& GetInstance();
	};
}