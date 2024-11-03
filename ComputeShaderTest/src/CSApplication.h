#pragma once
#include "CSWindow.h"
#include "Timer/YoiTimer.h"

class yoi::Window;
class yoi::YoiTimer;

class Application
{
public:
	Application();
	Application(unsigned int width, unsigned int height);
	int Run();
private:
	void DoFrame();
private:
	CSWindow wnd;
	yoi::YoiTimer timer;
};