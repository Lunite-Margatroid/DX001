#pragma once
#include "Texture.h"
#include "CubeTexture.h"
/**********************************************************************************
* Texture Manager will be a Singleton Class.
* It has 3 default textures at index 0, 1 and 2.
* The textures formats from one-pixel imgs.
* texture 0 is white: [1.0f, 1.0f, 1.0f, 1.0f]. It is default texture for diffuse and specular texture.
* texture 1 is green: [0.0f, 1.0f, 0.0f, 1.0f]. It is default texture for normal texture.
* texture 2 is black: [0.0f, 0.0f, 0.0f, 1.0f]. It is defulat texture for height texture.
***********************************************************************************/


namespace yoi
{
	class TextureManager
	{
		struct TextureItem
		{
			std::string title;
			BasicTexture2D* tex;
			TextureItem(const std::string& ttl, BasicTexture2D* t) :
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

		// Add texture to manager. this func is unsafe.
		BasicTexture2D* AddTexture(BasicTexture2D* tex, const std::string& title);

		template<typename Ttex, typename ... Args>
		Ttex* CreateTexture(Args ... args)
		{
			int n = m_Textures.size();
			char tempStr[32] = {};
			char texStr[64] = "texture-";
			_itoa(n, tempStr, 10);
			strcat(texStr, tempStr);


			Ttex* tex = new Ttex(args...);
			BasicTexture2D* bTex = dynamic_cast<BasicTexture2D*>(tex);
			assert(bTex);
			
			m_Textures.emplace_back(std::string(texStr), bTex);
			return tex;
		}

		void LogTextureInfo();
		BasicTexture2D* GetAt(int index = 0);
		unsigned int GetTexCount() const;
	};
}