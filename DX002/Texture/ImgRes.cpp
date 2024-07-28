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
		_wfopen_s(&f, wstr.c_str(), L"rb");	// ¿í×Ö·û°æµÄopen file

		if (f)
		{
			m_Data = stbi_load_from_file(f, &m_Width, &m_Height, &m_Channals, 4);
			fclose(f);
			m_Channals = 4;
			std::ostringstream oss;
			oss << "[Image Loaded]" << std::endl
				<< "\tFile Path:" << path << std::endl
				<< "\tWidht:  " << m_Width << std::endl
				<< "\tHeight: " << m_Height << std::endl
				<< "\tChannal:" << m_Channals;
			FileLogger::Info(oss.str().c_str());
			FileLogger::Flush();

			/*std::ostringstream out;
			out << std::endl;
			for (int i = 0; i < m_Height; i++)
			{
				for (int j = 0; j < m_Width; j++)
				{
					unsigned char* data = (unsigned char*)(((unsigned int*)m_Data) + j + i * m_Width);
					out << '(' << (int)data[0] << ',' << (int)data[1] << ',' << (int)data[2] << ',' << (int)data[3] << ')';
				}
				out << std::endl;
			}
			FileLogger::Info(out.str().c_str());
			FileLogger::Flush();*/
		}
		if (m_Data == 0)
		{
			std::ostringstream oss;
			oss << "Can't load file: " << path;
			FileLogger::Error(oss.str().c_str());
			FileLogger::Flush();
		}
	}
	ImgRes::~ImgRes()
	{
		Release();
	}
}