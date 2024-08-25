#include "pch.h"
#include "CSWindow.h"

CSWindow::CSWindow(int width, int height, const char* title)
	:Window(title, width, height)
{
	CSGraphics* gfx = new CSGraphics(yoi::Window::GetHandle());

	pGfx = std::unique_ptr<yoi::Graphics>(
		dynamic_cast<yoi::Graphics*>(gfx)
		);
}
