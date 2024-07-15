#include "pch.h"
#include "ImgRes.h"
#include <wchar.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"
#include "Logger\FileLogger.h"


#include "Shader\Shader.h"

namespace yoi
{
	int ImgRes::GetWidth() const
	{
		return m_Width;
	}
	int ImgRes::GetHeight() const
	{
		return m_Height;
	}
	int ImgRes::GetChannal() const
	{
		return m_Channals;
	}
	const unsigned char* ImgRes::GetDataPtr() const
	{
		return m_Data;
	}
	void ImgRes::Release()
	{
		if (m_Data)
			stbi_image_free(m_Data);
	}
	ImgRes::ImgRes(const std::string& path)
	{
		std::wstring wstr = UTF8ToWStr(path);

		FILE* f;
		_wfopen_s(&f, wstr.c_str(), L"rb");	// ���ַ����open file

		if (f)
		{
			m_Data = stbi_load_from_file(f, &m_Width, &m_Height, &m_Channals, 0);
			fclose(f);
		}
		if (m_Data == 0)
		{
			std::ostringstream oss;
			oss << "Can't load file: " << path;
			FileLogger::Error(oss.str().c_str());
		}
	}
	ImgRes::~ImgRes()
	{
		Release();
	}
}