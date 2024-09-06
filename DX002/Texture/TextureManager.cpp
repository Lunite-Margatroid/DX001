#include "pch.h"
#include "TextureManager.h"
#include "Logger\FileLogger.h"
#include <iomanip>
namespace yoi
{
	TextureManager::TextureManager()
	{
		float white[4] = {1.0f, 1.0f,1.0f,1.0f};
		LoadTexture(white, std::string("single-color while"));
	}
	TextureManager::~TextureManager()
	{
		for (TextureItem& item : m_Textures)
		{
			delete item.tex;
		}
	}
	Texture* TextureManager::LoadTexture(const std::string& path)
	{
		std::string tPaht = path;
		std::replace(tPaht.begin(), tPaht.end(), '\\', '/');
		int pos = static_cast<int>(tPaht.find_first_of('/'));
		if (pos < 0) pos = 0;
		Texture* texture = new Texture(tPaht);
		m_Textures.emplace_back(
			TextureItem(
				tPaht.substr(pos + 1),
				texture
		));
		return texture;
	}
	Texture* TextureManager::LoadTexture(const float* color, const std::string& title)
	{
		Texture* texture = new Texture(color);
		m_Textures.emplace_back(
			TextureItem(
				title,
				texture
			));
		return texture;
	}
	Texture* TextureManager::LoadTexture(ID3D11Device* device, const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* data)
	{
		Texture* texture = new Texture(device, desc, data);

		char titleBuffer[16] = "Tex";
		_itoa(static_cast<int>(m_Textures.size() + 1), titleBuffer + 3, 10);

		m_Textures.emplace_back(
			TextureItem(titleBuffer, texture)
		);
		return texture;
	}
	void TextureManager::LogTextureInfo()
	{
		std::ostringstream oss;
		oss << std::endl
			<< "Texture Count: " << m_Textures.size() << std::endl;

		int count = 0;
		for (TextureItem& item : m_Textures)
		{
			D3D11_TEXTURE2D_DESC td = {};
			item.tex->GetTexture2D()->GetDesc(&td);

			count++;
			int width = td.Width;
			int height = td.Height;
			oss <<'[' <<count << ']' << std::setw(3) <<item.title << std::setw(16) << "Width: " << std::setw(5) << width << "Height: " << std::setw(5) << height << std::endl;
		}

		FileLogger::Info(oss.str().c_str());
		FileLogger::Flush();
	}
	BasicTexture2D* TextureManager::GetAt(int index)
	{
		if (index < m_Textures.size() )
			return m_Textures[index].tex;
		FileLogger::Error("Texture Index out of range.");
		FileLogger::Flush();
		return nullptr;
	}
	unsigned int TextureManager::GetTexCount() const
	{
		return m_Textures.size();
	}
}