#pragma once
#include "Material.h"

namespace yoi
{
	class MaterialManager
	{
		struct MaterialItem
		{
			std::string title;
			Material* material;
			MaterialItem(const std::string ttl, Material* mtl)
				:title(ttl), material(mtl)
			{}
		};
	private:
		std::vector<MaterialItem> m_Materials;
	public:
		MaterialManager();
		~MaterialManager();
		MaterialManager(const MaterialManager&) = delete;
		MaterialManager& operator = (const MaterialManager&) = delete;

		Material* Add(Material* material, const std::string& title);

		Material* At(int n);
	};
}