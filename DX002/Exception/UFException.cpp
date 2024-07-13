#include "pch.h"
#include "UFException.h"

namespace yoi
{
	UFException::UFException(int line, const char* file) noexcept
		:YoiException(line, file)
	{
	}
	const char* UFException::GetType() const noexcept
	{
		return "A Function has not been finished";
	}
}
