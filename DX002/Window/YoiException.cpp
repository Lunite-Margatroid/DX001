#include "pch.h"
#include "YoiException.h"
#include "Logger/FileLogger.h"

namespace yoi
{
	YoiException::YoiException(int line, const char* file) noexcept
		:line(line), file(file)
	{}

	const char* YoiException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< GetOriginString();
		whatBuffer = std::move(oss.str());
		FileLogger::Error(whatBuffer.c_str());
		return whatBuffer.c_str();
	}

	const char* YoiException::GetType()const noexcept
	{
		return "Yoi Exception";
	}

	int YoiException::GetLine() const noexcept
	{
		return line;
	}

	const std::string& YoiException::GetFile() const noexcept
	{
		return file;
	}

	std::string YoiException::GetOriginString() const noexcept
	{
		return std::string(std::exception::what());
	}
}