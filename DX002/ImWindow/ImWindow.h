#pragma once
#include "ImGui\imgui.h"

namespace yoi
{
	class ImWindow
	{
	protected:
		ImGuiIO& m_IO;
		bool m_Open;
	public:
		ImWindow();
		~ImWindow() = default;
		ImWindow(const ImWindow&) = delete;
		ImWindow& operator = (const ImWindow&) = delete;

		virtual void ShowWindow() = 0;
		virtual const char* GetWindowTitle() const = 0;
	};
}