#pragma once
#include "MyWin.h"
#include "YoiException.h"
#include "resource.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "Renderer/Graphics.h"



namespace yoi
{
	class Window
	{
	private:
		class WindowClass
		{
		public:
			static const char* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass();

			WindowClass(const WindowClass&) = delete;
			WindowClass& operator = (const WindowClass&) = delete;
			static constexpr const char* wndClassName = "Lunite Direct3D Windowclass";
			static WindowClass wndClass;
			HINSTANCE hInst;
		};
	public:
		class Exception : public YoiException
		{
		public:
			Exception(int line, const char* file, HRESULT hr) noexcept;
			const char* what() const noexcept override;
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
		private:
			HRESULT hr;
		};
	public:
		Window(int width, int height, const char* name) noexcept;
		~Window();

		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	public:
		void SetTitle(const std::string & title);
		static std::optional<int> ProcessMessage();

	private:
		int width;
		int height;
		HWND hWnd;

	public:
		KeyBoard kbd;
		Mouse mouse;
	private:
		std::unique_ptr<Graphics> pGfx;
	public:
		HWND GetHandleWindow();
		Graphics& Gfx();
	};
}

#define YOIWND_EXCEPT(hr) yoi::Window::Exception(__LINE__, __FILE__, hr)
#define YOIWND_LAST_EXCEPT() yoi::Window::Exception(__LINE__, __FILE__, GetLastError())