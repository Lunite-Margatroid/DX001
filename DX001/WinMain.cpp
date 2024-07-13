#include <Windows.h>


LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	LPCWSTR MyWindowClassName = L"PrimaryWindow";
	// register the window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = MyWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MyWindowClassName;
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);
	// create window instance
	HWND hWindow = CreateWindowEx(
		NULL,
		wc.lpszClassName, 
		L"Hello DX",
		WS_OVERLAPPEDWINDOW,
		200, 
		200,
		640,
		400,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// show window
	ShowWindow(hWindow, SW_SHOW);

	MSG msg;
	BOOL bResult;

	while ((bResult = GetMessage(&msg, hWindow, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return bResult;
}

LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(250);
		break;
	case WM_KEYDOWN:
		if(wParam == 'A')
			SetWindowTextA(hWnd, "Hello Message");
		break;
	};
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}