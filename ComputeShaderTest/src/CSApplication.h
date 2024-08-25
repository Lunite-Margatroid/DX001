#pragma once
#include "CSWindow.h"
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
	CSWindow wnd;
	yoi::YoiTimer timer;
};