#pragma once
#include "WTWindow.h"
#include "Timer/YoiTimer.h"

#define YOI_WINDOW_WIDTH 800
#define YOI_WINDOW_HEIGHT 600


class yoi::Window;
class yoi::YoiTimer;

class Application
{
public:
	Application();
	int Run();
private:
	void DoFrame();
private:
	WTWindow wnd;
	yoi::YoiTimer timer;
};