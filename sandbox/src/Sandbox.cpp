#include "Sandbox.h"

namespace yoi
{

	yoi::Sandbox::Sandbox()
		:Application(1280, 720, "sandbox")
	{
		wnd.Gfx().InitPipeline();
	}
	void Sandbox::DoFrame()
	{
		wnd.Gfx().Update();
		wnd.Gfx().RunPipeline();
		wnd.Gfx().ImGuiFrame();
		wnd.Gfx().EndFrame();
	}
}
