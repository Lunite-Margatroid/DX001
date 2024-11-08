#pragma once

namespace yoi
{
	class ImgRes
	{
	private:
		unsigned char* m_Data;
		int m_Channals;
		int m_Width;
		int m_Height;
		int m_ResultCode;
	public:
		ImgRes(const std::string& path);
		ImgRes() = delete;
		~ImgRes();
		ImgRes(const ImgRes&) = delete;
		ImgRes& operator = (const ImgRes&) = delete;

		int GetWidth() const;
		int GetHeight() const;
		int GetChannal() const;
		const unsigned char* GetDataPtr() const;
		void Release();
		void ErrorColor();

		bool Resize(int newSizeWidth, int newSizeHeight);

	};
}