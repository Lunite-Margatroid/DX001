#pragma once
#include "Window/Window.h"
#include "Timer/YoiTimer.h"
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