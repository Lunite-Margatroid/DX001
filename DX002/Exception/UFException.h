#pragma once
#include "Window/YoiException.h"

#ifdef YOI_DEBUG
#define YOI_THROW_NOT_FINISHED_FUNCTION() throw yoi::UFException(__LINE__, __FILE__)
#else
#define YOI_THROW_NOT_FINISHED_FUNCTION()
#endif // YOI_DEBUG



namespace yoi
{
	class UFException : public YoiException
	{
	public:
		UFException(int line, const char* file) noexcept;
		virtual const char* GetType() const noexcept;
	};
}