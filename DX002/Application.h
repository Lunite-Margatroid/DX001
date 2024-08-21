#pragma once
#include "Window/Window.h"
#include "Timer/YoiTimer.h"

#define YOI_WINDOW_WIDTH 800
#define YOI_WINDOW_HEIGHT 600

namespace yoi
{
	class Application
	{
	public:
		Application();
		int Run();
	private:
		void DoFrame();
	private:
		Window wnd;
		YoiTimer timer;
	};
}