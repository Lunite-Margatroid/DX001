#pragma once
#include "Window\Window.h"
#include "WTGraphics.h"

class WTWindow : public yoi::Window
{
public:
	WTWindow(int width, int height, const char* title);
};