#pragma once
#include "Window/Window.h"
#include "Timer/YoiTimer.h"



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