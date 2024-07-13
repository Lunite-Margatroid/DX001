#include "pch.h"
#include "Application.h"

namespace yoi
{
	Application::Application() :
		wnd(800, 600, "DX002")
	{}

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
		float c = sinf(timer.Peek()) / 2.f + 0.5f;
		wnd.Gfx().ClearBuffer(c, c, 1.0f);
		
		wnd.Gfx().DrawTriangle();
		wnd.Gfx().ImGuiFrame();
		wnd.Gfx().EndFrame();
	}
}