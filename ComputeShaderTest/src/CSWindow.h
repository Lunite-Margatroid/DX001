#pragma once
#include "Window\Window.h"
#include "CSGraphics.h"
class CSWindow : public yoi::Window
{
public:
	CSWindow(int width, int height, const char* title);
};