#pragma once
#include "MyWin.h"
#include "Window/YoiException.h"
#include "InfoManager/DxgiInfoManager.h"
#include "Timer/YoiTimer.h"
#include <D3D11.h>
#include <wrl.h>
#include "ImGuiManager/ImGuiManager.h"
#include "Sprite\SpriteV1Cube.h"
#include "SceneObject\PerspectiveCamera.h"
#include "ImWindow\ObjectPropertyWin.h"
#include "Shader\ShaderManager.h"

namespace yoi
{
	class Graphics
	{
		friend class Sprite;
	public:
		class Exception : public YoiException
		{
			using YoiException::YoiException;
		};

		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;

			static std::string TranslateErrorCode(HRESULT hr) noexcept;
		private:
			HRESULT hr;
			std::string info;
		};

		class InfoException : public Exception
		{
		public:
			InfoException(int line, const char* file,std::vector<std::string> infoMsgs) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			std::string GetErrorInfo() const noexcept;
		private:
			std::string info;
		};

		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			const char* GetType() const noexcept override;
			const char* what() const noexcept override;
		private:
			std::string reason;
		};

	public:
		Graphics(HWND hWnd);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics() = default;
		
		void ImGuiFrame();
		void EndFrame();

		void ClearBuffer(float red, float green, float blue);

	/****** test draw ******/
	private:
		
		std::unique_ptr<SceneObj> m_RootObj;
		CameraObj* m_MainCamera;

		YoiTimer timer;

		std::unique_ptr<ObjectPropertyWin> m_pObjWin;

	public:
		void InitTestDraw();
		void DrawTriangle();
	/****** test draw * end */


	private:
		/*ID3D11Device* pDevice = nullptr;
		IDXGISwapChain* pSwap = nullptr;
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11RenderTargetView* pTarget = nullptr;*/

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		/*
		* 使用&运算符，获取的是ComPtr的资源的地址，即内部指针的地址。
		* 使用&运算符，ComPtr会先Release，再返回指针的地址。这意味着使用&后，原先的实例会失效。
		* 使用GetAddressOf()获取指针的地址，不会释放已经使用的内存。
		* 使用Get()获取指针。
		* 使用ReleaseAndGetAddressOf()和&运算符相同。
		*/

#ifdef  YOI_DEBUG
	private:
		DxgiInfoManager infoManager;
	public:
		DxgiInfoManager& GetInfoManager();
#endif //  YOI_DEBUG

		/**** ImGui ****/
		private:
			std::unique_ptr<ImGuiManager> pImGuiManager;

		/****** Singleton? ******/
		// only single window accepted
		private:
			static Graphics* s_pInstance;
		public:
			static Graphics& GetInstance();

		/******* Shader **********/
		private:
			std::unique_ptr<ShaderManager> m_pShaderManager;
		public:
			Shader* GetShader(const std::string& shaderTitle);
	};

}