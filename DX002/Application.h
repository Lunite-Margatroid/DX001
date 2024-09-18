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
		Application(unsigned int width, unsigned int height, const char* title);
		virtual int Run();
	protected:
		virtual void DoFrame();
	protected:
		Window wnd;
		YoiTimer timer;
	};
}