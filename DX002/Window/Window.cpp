#include "pch.h"
#include "Window.h"
#include <d3d11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace yoi
{
	// ------------------ Window Class -----------------------
	Window::WindowClass Window::WindowClass::wndClass;

	Window::WindowClass::WindowClass() noexcept
		:hInst(GetModuleHandle(NULL))
	{
		WNDCLASSEXA wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = reinterpret_cast<HICON> (LoadImageA(hInst, MAKEINTRESOURCEA(APP_ICON), IMAGE_ICON, 256, 256, 0));
		wc.hCursor = NULL;
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = wndClassName;
		wc.hIconSm = reinterpret_cast<HICON> (LoadImageA(hInst, MAKEINTRESOURCEA(APP_ICON), IMAGE_ICON, 256, 256, 0));
		RegisterClassExA(&wc);
	}

	Window::WindowClass::~WindowClass()
	{
		UnregisterClassA(wndClassName, GetInstance());
	}

	const char* Window::WindowClass::GetName() noexcept
	{
		return wndClassName;
	}

	HINSTANCE Window::WindowClass::GetInstance() noexcept
	{
		return wndClass.hInst;
	}

	// ------------------ Window exception ------------------
	Window::Exception::Exception(
		int line, const char* file, HRESULT hr)
		noexcept
		:YoiException(line, file),hr(hr)
	{}
	
	const char* Window::Exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorString() << std::endl
			<< GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf),
			0,
			nullptr);
		if (nMsgLen == 0)
		{
			return "Unidentified error code";
		}
		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	HRESULT Window::Exception::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string Window::Exception::GetErrorString()const noexcept
	{
		return TranslateErrorCode(hr);
	}


	// -------------------- Window --------------------------
	Window::Window(const char* name, int width, int height)
		:width(width), height(height)
	{
		// ���崰��λ��
		RECT rect;
		rect.left = 100;
		rect.right = width + rect.left;
		rect.top = 100;
		rect.bottom = height + rect.top;
		// ������Ч�ͻ���
		AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			FALSE);
		// exception test 



		// create window
		hWnd = CreateWindowA(
			WindowClass::GetName(),
			name,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr,
			nullptr,
			WindowClass::GetInstance(),
			this
		);
		// show window
		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}

	Window::Window(int width, int height, const char* name) noexcept
		:width(width), height(height)
	{
		// ���崰��λ��
		RECT rect;
		rect.left = 100;
		rect.right = width + rect.left;
		rect.top = 100;
		rect.bottom = height + rect.top;
		// ������Ч�ͻ���
		AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			FALSE);
		// exception test 
		


		// create window
		hWnd = CreateWindowA(
			WindowClass::GetName(),
			name,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr,
			nullptr,
			WindowClass::GetInstance(),
			this
		);
		// show window
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		// create graph
		pGfx = std::make_unique<Graphics>(hWnd);
	}

	Window::~Window()
	{
		pGfx.release();
		DestroyWindow(hWnd);
	}

	LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// ����������ǰ����Ϣ
		// ���û�����Ĵ���ʵ����ָ�봫��win����ʵ����ָ�����
		if (msg == WM_NCCREATE)
		{
			// ����Ϣ�����л�ô��ڴ�����Ϣ
			const CREATESTRUCT* const pCreate =
				reinterpret_cast<CREATESTRUCT*>(lParam);

			// �Ӵ��ڴ�����Ϣ�л��ex����
			// ��CreateWindow������û�����Ĵ���ʵ����ָ��
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

			// ���û�����Ĵ���ʵ����ָ�봫��win����ʵ��
			// �����ͽ�����һ��һ��ӳ��
			// ����ͨ�����ھ���õ��û�����Ĵ���ʵ��
			// Ҳ����ͨ���û�����Ĵ���ʵ���õ����ھ��
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

			// ����������Ϣ������
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&HandleMsgThunk));

			// ������Ϣ�����û��������Ϣ������ (���ܻ���Ҫ�����Ĵ���)
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// Ĭ�ϴ���WM_NCCREATE֮ǰ����Ϣ
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// ͨ�����ھ������û�����Ĵ���ʵ��
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// ���øô��ڵ���Ϣ������
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

		switch (msg)
		{
		/* ----------------- key board msg ------------------------ */
		case WM_KEYDOWN:
			if (!(lParam & 0x40000000) || kbd.autorepeatEnabled)
			{
				kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_KEYUP:
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;

		// ϵͳ��������Ϣ��Ҫ WM_SYSKEYDOWN
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || kbd.autorepeatEnabled)
			{
				kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_SYSKEYUP:
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			kbd.OnChar(static_cast<char>(wParam));
			break;

		// ����Ӹô�����ʧ �������״̬
		case WM_KILLFOCUS:
			kbd.ClearState();
			break;

		/* ----------------- key board msg  ------------------------ end */
		
		/* ------------------ mouse msg ------------------------------ */

		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(pt.x, pt.y);
			break; 
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(pt.x, pt.y);
			break; 
		}
		case WM_RBUTTONDOWN:
		{	
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightReleased(pt.x, pt.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.OnWheelDelta(pt.x, pt.y, delta);
			break;
		}
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);

			// cursor move �� �ͻ����� �ƶ��� �ͻ�����
			// �����ƶ�֮�� ��Ҫ���ͽ���ͻ�����Ϣ
			if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
			{
				mouse.OnMouseMove(pt.x, pt.y);
				if (!mouse.IsInWindow())
				{
					SetCapture(hWnd);
					mouse.OnMouseEnter();
				}
			}
			else
			{
				// �������ڿͻ���֮�� ����갴������
				// ���ڼ�����ȡ�����Ϣ
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					mouse.OnMouseMove(pt.x, pt.y);
				}
				else
				{
					// �������ڿͻ���֮���Ұ����ͷ�
					// �����ͷ���� ���ڻ�ȡ�����Ϣ
					// ������ �Ƴ��ͻ�����Ϣ
					ReleaseCapture();
					mouse.OnMouseLeave();
				}
			}

			// Mouse Message Test
			/*std::ostringstream oss;
			oss << "cursor pos: " << "(" << pt.x << ", " << pt.y << ")";
			SetWindowTextA(hWnd, oss.str().c_str());*/

			break;
		}
		/* ------------------ mouse msg ------------------------------ end */
		
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void Window::SetTitle(const std::string& title)
	{
		SetWindowTextA(hWnd, title.c_str());
	}

	HWND Window::GetHandleWindow()
	{
		return hWnd;
	}

	Graphics& Window::Gfx()
	{
		return *pGfx;
	}

	Graphics* Window::GetGraphisPtr()
	{
		return pGfx.get();
	}

	std::optional<int> Window::ProcessMessage()
	{
		MSG msg;
		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return static_cast<int>(msg.wParam);
			}
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return std::nullopt;
	}

	HWND Window::GetHandle()
	{
		return hWnd;
	}
}