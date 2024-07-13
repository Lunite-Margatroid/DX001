#pragma once
#include "ImGui/imgui.h"
#include "MyWin.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

namespace yoi
{
	class ImGuiManager
	{
	private:
		ImGuiIO* m_IO;
	public:
		ImGuiManager(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		~ImGuiManager();

		void BeginImGuiFrame();
		void EndImGuiFrame();
	};
}
