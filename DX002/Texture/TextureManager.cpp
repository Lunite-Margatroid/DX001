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
	void TextureManager::LogTextureInfo()
	{
		std::ostringstream oss;
		oss << std::endl
			<< "Texture Count: " << m_Textures.size() << std::endl;

		int count = 0;
		for (TextureItem& item : m_Textures)
		{
			count++;
			int width = item.tex->m_Width;
			int height = item.tex->m_Height;
			oss <<'[' <<count << ']' << std::setw(3) <<item.title << std::setw(16) << "Width: " << std::setw(5) << width << "Height: " << std::setw(5) << height << std::endl;
		}

		FileLogger::Info(oss.str().c_str());
		FileLogger::Flush();
	}
	Texture* TextureManager::GetAt(int index)
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