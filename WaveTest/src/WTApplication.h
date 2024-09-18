#pragma once
#include "Timer/YoiTimer.h"
#include "Application.h"

#define WAVE_TEST_WINDOW_WIDTH 1280
#define WAVE_TEST_WINDOW_HEIGHT 720


class yoi::Window;
class yoi::YoiTimer;

class WTAApplication : public yoi::Application
{
public:
	WTAApplication();
	int Run() override;
private:
	void DoFrame() override;
};