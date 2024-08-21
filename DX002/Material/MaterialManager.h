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

		unsigned int Size() const;

		Material* Add(Material* material, const std::string& title);

		Material* At(int n);

		template<typename ... Args>
		Material* CreateMaterial(Args...args)
		{
			Material* material = new Material(args...);
			int n = m_Materials.size();
			char buf[16];
			_itoa(n, buf,10);
			Add(material, std::string(buf));
			return material;
		}
	};
}