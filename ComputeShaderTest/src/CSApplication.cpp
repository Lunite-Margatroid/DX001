#include "pch.h"
#include "MyWin.h"
#include "CSApplication.h"



Application::Application() :
	wnd(YOI_WINDOW_WIDTH, YOI_WINDOW_HEIGHT, "CouputeShaderTest")
{
	// wnd.Gfx().InitTestDraw();
	wnd.Gfx().InitPipeline();
}

Application::Application(unsigned int width, unsigned int height)
	:wnd(width, height, "CouputeShaderTest")
{
	wnd.Gfx().InitPipeline();
}

int Application::Run()
{
	while (true)
	{
		if (const auto result = yoi::Window::ProcessMessage())
		{
			return *result;
		}
		DoFrame();
	}
	return 0;
}

void Application::DoFrame()
{

	wnd.Gfx().Update();
	wnd.Gfx().RunPipeline();
	wnd.Gfx().ImGuiFrame();
	wnd.Gfx().EndFrame();
}
