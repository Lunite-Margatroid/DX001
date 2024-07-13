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
		* ʹ��&���������ȡ����ComPtr����Դ�ĵ�ַ�����ڲ�ָ��ĵ�ַ��
		* ʹ��&�������ComPtr����Release���ٷ���ָ��ĵ�ַ������ζ��ʹ��&��ԭ�ȵ�ʵ����ʧЧ��
		* ʹ��GetAddressOf()��ȡָ��ĵ�ַ�������ͷ��Ѿ�ʹ�õ��ڴ档
		* ʹ��Get()��ȡָ�롣
		* ʹ��ReleaseAndGetAddressOf()��&�������ͬ��
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