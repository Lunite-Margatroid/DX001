#include "pch.h"
#include "WTWindow.h"

WTWindow::WTWindow(int width, int height, const char* title)
	:Window(title, width, height)
{
	WTGraphics* gfx = new WTGraphics(yoi::Window::GetHandle());

	pGfx = std::unique_ptr<yoi::Graphics>(
		dynamic_cast<yoi::Graphics*>(gfx)
	);
}
