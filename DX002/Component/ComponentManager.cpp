#include "pch.h"
#include "ComponentManager.h"
namespace yoi
{
	ComponentManager::ComponentManager()
	{
	}
	ComponentManager& ComponentManager::GetInstance()
	{
		static ComponentManager inst;
		return inst;
	}
}