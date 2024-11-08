#include "pch.h"
#include "ImgRes.h"
#include <wchar.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"
#include "Logger\FileLogger.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb\stb_image_resize.h"

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
		if (m_ResultCode >= 0)
			stbi_image_free(m_Data);
		else
			delete[] m_Data;
	}
	void ImgRes::ErrorColor()
	{
		m_Data = new unsigned char[16];
		unsigned char buf[16] = {
			0,0,0,255,
			255,0,255,255,
			255,0,255,255,
			0,0,0,255
		};
		memcpy(m_Data, buf, 16);
		m_Channals = 4;
		m_Height = 2;
		m_Width = 2;
	}
	bool ImgRes::Resize(int newSizeWidth, int newSizeHeight)
	{
		if (m_ResultCode < 0 || m_Data == nullptr)
			return false;

		unsigned char* tempData = new unsigned char[newSizeHeight * newSizeWidth * m_Channals];
		stbir_resize_uint8(m_Data, m_Width, m_Height, 0, tempData, newSizeWidth, newSizeHeight, 0, m_Channals);
		Release();
		m_Data = tempData;
		m_Width = newSizeWidth;
		m_Height = newSizeHeight;
		return true;
	}
	ImgRes::ImgRes(const std::string& path)
	{
		m_Data = nullptr;
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

			m_ResultCode = 0;

			if (m_Data == nullptr)
			{
				std::ostringstream oss;
				oss << "Can't load file: " << path;
				FileLogger::Error(oss.str().c_str());
				FileLogger::Flush();
				m_ResultCode = -2;
				ErrorColor();
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "Can't find file: " << path;
			FileLogger::Error(oss.str().c_str());
			FileLogger::Flush();
			m_ResultCode = -1;
			ErrorColor();
		}
		
	}
	ImgRes::~ImgRes()
	{
		Release();
	}
}