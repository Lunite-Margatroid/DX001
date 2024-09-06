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
			BasicTexture2D* tex;
			TextureItem(const std::string& ttl, Texture* t) :
				title(ttl), tex(t) {}
		};
	private:
		std::vector<TextureItem> m_Textures;
	public:
		TextureManager();
		~TextureManager();
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator = (const TextureManager&) = delete;
		Texture* LoadTexture(const std::string& path);
		Texture* LoadTexture(const float* color, const std::string& title = std::string("No Title"));
		Texture* LoadTexture(ID3D11Device*, const D3D11_TEXTURE2D_DESC*, const D3D11_SUBRESOURCE_DATA* );
		void LogTextureInfo();
		BasicTexture2D* GetAt(int index = 0);
		unsigned int GetTexCount() const;
	};
}