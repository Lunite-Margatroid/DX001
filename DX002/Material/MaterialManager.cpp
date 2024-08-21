#include "pch.h"
#include "Material\MaterialManager.h"

namespace yoi
{

	MaterialManager::MaterialManager()
	{
	}

	MaterialManager::~MaterialManager()
	{
		for (MaterialItem& item : m_Materials)
		{
			delete item.material;
		}
	}

	unsigned int MaterialManager::Size() const
	{
		return m_Materials.size();
	}

	Material* MaterialManager::Add(Material* material, const std::string& title)
	{
		m_Materials.emplace_back(MaterialItem(title, material));
		return m_Materials.back().material;
	}

	Material* MaterialManager::At(int n)
	{
		if (n >= m_Materials.size())
			return nullptr;
		return m_Materials[n].material;
	}

}
