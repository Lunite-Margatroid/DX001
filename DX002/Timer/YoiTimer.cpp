#include "pch.h"
#include "YoiTimer.h"


namespace yoi
{
	YoiTimer::YoiTimer()
	{
		last = std::chrono::steady_clock::now();
	}

	float YoiTimer::Mark()
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> frameTime = last - old;
		return frameTime.count();
	}

	float YoiTimer::Peek() const
	{
		return std::chrono::duration<float>(
			std::chrono::steady_clock::now() - last
		).count();
	}
}