#pragma once
#include <chrono>
namespace yoi
{
	class YoiTimer
	{
	public:
		YoiTimer();
		float Mark();
		float Peek() const;
	private:
		std::chrono::steady_clock::time_point last;
	};
}