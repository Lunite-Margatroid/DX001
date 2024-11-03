/*
* the ImGui window to render a DX11 texture. As the same time, it controlls a 3D camera.
*/

#pragma once
#include "ImWindow.h"
#include <d3d11.h>
#include <wrl.h>
#include "SceneObject\Camera3DObj.h"

namespace yoi
{
	class MainSceneWin : public ImWindow
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;
		Camera3DObj* m_Camera;

		float m_AspectRatio;
	
		float m_DeltaTime;
		ImVec2 m_LastMosPos;
		ImVec2 m_ClickedPos;
	public:
		MainSceneWin() = delete;
		MainSceneWin(const MainSceneWin&) = delete;
		MainSceneWin& operator = (const MainSceneWin&) = delete;
		~MainSceneWin() = default;

		MainSceneWin(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pResView, Camera3DObj* camera);

		void ShowWindow() override;
		const char* GetWindowTitle() const override;
		void Update(float deltaTime) override;
	};
}