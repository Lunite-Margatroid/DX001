#pragma once
#include "Texture.h"
/**********************************************************************************
* Texture Manager will be a Singleton Class.
* It has a default texture at index 0. This texture formats from an white one-pixel img.
***********************************************************************************/


namespace yoi
{
	class TextureManager
	{
		struct TextureItem
		{
			std::string title;
			Texture* tex;
			TextureItem(const std::string& ttl, Texture* t) :
				title(ttl), tex(t) {}
		};
	private:
		std::vector<TextureItem> m_Textures;
	public:
		TextureManager();
		~TextureManager();
		TextureManager(const Texture&) = delete;
		TextureManager& operator = (const TextureManager&) = delete;
		Texture* LoadTexture(const std::string& path);
		Texture* LoadTexture(const float* color, const std::string& title = std::string("No Title"));
		void LogTextureInfo();
		Texture* GetAt(int index = 0);
	};
}