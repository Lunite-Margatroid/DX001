#include "pch.h"
#include "MainSceneWin.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	MainSceneWin::MainSceneWin(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pResView, Camera3DObj* camera)
		:m_Texture(pResView), m_Camera(camera), m_DeltaTime(0.0f), m_LastMosPos(0.0f, 0.0f)
	{
		GFX_EXCEPT_SUPPORT();

		ID3D11Texture2D* texResource;

		GFX_THROW_INFO_ONLY(m_Texture->GetResource((ID3D11Resource**)&texResource));

		D3D11_TEXTURE2D_DESC td = {};
		texResource->GetDesc(&td);
		m_AspectRatio = (float)td.Width / td.Height;

	}
	void MainSceneWin::ShowWindow()
	{
		ImGui::Begin(GetWindowTitle(), &m_Open);
		ImTextureID mainTexture = m_Texture.Get();

		ImVec2 windowPos = ImGui::GetCursorScreenPos();					// 当前窗口位置
		ImVec2 windowSize = ImGui::GetWindowSize();						// 窗口大小
		ImVec2 leftTop, rightBottom, texSize;
		texSize = windowSize;
		leftTop.x = windowSize.x / 2 + windowPos.x;
		leftTop.y = windowSize.y / 2 + windowPos.y;
		float winAspect = windowSize.x / windowSize.y;
		if (winAspect > m_AspectRatio)
		{
			texSize.x = texSize.y * m_AspectRatio;
		}
		else
		{
			texSize.y = texSize.x / m_AspectRatio;
		}
		leftTop.x -= texSize.x / 2;
		leftTop.y -= texSize.y / 2;
		rightBottom.x = leftTop.x + texSize.x;
		rightBottom.y = leftTop.y + texSize.y;
		
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		if(mainTexture)
			drawList->AddImage(mainTexture, leftTop, rightBottom);

		if (ImGui::IsWindowFocused())
		{
			// key control
			ImGuiIO& io = ImGui::GetIO();
			Camera3DObj* camera = m_Camera;

			if (camera)
			{
				if (io.KeysDown[ImGuiKey_W])
					camera->Move(CameraObj::Direction::front, m_DeltaTime);
				if (io.KeysDown[ImGuiKey_A])
					camera->Move(CameraObj::Direction::left, m_DeltaTime);
				if (io.KeysDown[ImGuiKey_S])
					camera->Move(CameraObj::Direction::back, m_DeltaTime);
				if (io.KeysDown[ImGuiKey_D])
					camera->Move(CameraObj::Direction::right, m_DeltaTime);
				if (io.KeysDown[ImGuiKey_LeftShift])
					camera->Move(CameraObj::Direction::down, m_DeltaTime);
				if (io.KeysDown[ImGuiKey_Space])
					camera->Move(CameraObj::Direction::up, m_DeltaTime);
			}
			// mouse control
			
			ImVec2 sDeltaPos;
				
			if (io.MouseDown[0] && camera)	// 左键按下
			{
				m_ClickedPos = io.MousePos;

				// 这里windowPos 是scene绘制区域的左上角
				if (m_ClickedPos.x >= leftTop.x &&
					m_ClickedPos.x <= rightBottom.x &&
					m_ClickedPos.y >= leftTop.y &&
					m_ClickedPos.y <= rightBottom.y )	// 左键点击位置在目标区域内
				{
					sDeltaPos.x = io.MousePos.x - m_LastMosPos.x;
					sDeltaPos.y = io.MousePos.y - m_LastMosPos.y;
					// std::cout << "Mouse Drag dleta Pos: " << sDeltaPos.x << ", " << sDeltaPos.y << std::endl;

					camera->RotateYaw(-sDeltaPos.x / 500.0f);
					camera->RotatePitch(-sDeltaPos.y / 500.0f);
				}
			}
		}
		m_LastMosPos = ImGui::GetMousePos();

		ImGui::End();
	}
	const char* MainSceneWin::GetWindowTitle() const
	{
		return "Main Scene";
	}
	void MainSceneWin::Update(float deltaTime)
	{
		m_DeltaTime = deltaTime;
		
	}
}