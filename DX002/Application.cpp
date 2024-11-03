#include "pch.h"
#include "Application.h"

namespace yoi
{
	Application::Application() :
		wnd(YOI_WINDOW_WIDTH, YOI_WINDOW_HEIGHT, "DX002")
	{
		wnd.Gfx().InitTestDraw();
		wnd.Gfx().InitPipeline();
	}

	Application::Application(unsigned int width, unsigned int height, const char* title)
		:wnd(width, height, title)
	{
		wnd.Gfx().InitTestDraw();
		wnd.Gfx().InitPipeline();
	}

	int Application::Run()
	{
		while (true)
		{
			if (const auto result = Window::ProcessMessage())
			{
				return *result;
			}
			DoFrame();
		}
		return 0;
	}

	void Application::DoFrame()
	{
		/*std::ostringstream oss;
		oss << "time has pasted: " << timer.Peek() << 's';
		wnd.SetTitle(oss.str());*/
		// float c = sinf(timer.Peek()) / 2.f + 0.5f;

		/*wnd.Gfx().ClearBuffer();
		
		wnd.Gfx().DrawTriangle();
		wnd.Gfx().ImGuiFrame();
		wnd.Gfx().EndFrame();*/

		wnd.Gfx().Update();
		wnd.Gfx().RunPipeline();
		wnd.Gfx().ImGuiFrame();
		wnd.Gfx().EndFrame();
	}
}