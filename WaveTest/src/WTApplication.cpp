#include "pch.h"
#include "MyWin.h"
#include "WTApplication.h"



WTAApplication::WTAApplication() :
	yoi::Application(WAVE_TEST_WINDOW_WIDTH, WAVE_TEST_WINDOW_HEIGHT, "wave test")
{
	
}

int WTAApplication::Run()
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

void WTAApplication::DoFrame()
{
	/*std::ostringstream oss;
	oss << "time has pasted: " << timer.Peek() << 's';
	wnd.SetTitle(oss.str());*/
	// float c = sinf(timer.Peek()) / 2.f + 0.5f;

	/*wnd.Gfx().ClearBuffer(0.224f, 0.773f, 0.733f);

	wnd.Gfx().DrawTriangle();
	wnd.Gfx().ImGuiFrame();
	wnd.Gfx().EndFrame();*/

	wnd.Gfx().Update();
	wnd.Gfx().RunPipeline();
	wnd.Gfx().ImGuiFrame();
	wnd.Gfx().EndFrame();
}
