#include "pch.h"
#include "ImWindow.h"

namespace yoi
{
	ImWindow::ImWindow()
		:m_IO(ImGui::GetIO()),
		m_Open(true)
	{
	}
}