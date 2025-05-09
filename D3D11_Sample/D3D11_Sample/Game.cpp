#include "Game.h"
#include <Windows.h>

HWND hWnd = NULL;
std::wstring WindowTitle = L"タイトル";
int ScreenWidth = 640;
int ScreenHeight = 480;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"情報", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool InitWindow(const std::wstring& windowTitle, int screenWidth, int screenHeight)
{
	const auto hInstance = GetModuleHandleW(NULL);
	WindowTitle = windowTitle;
	ScreenWidth = screenWidth;
	ScreenHeight = screenHeight;

	const wchar_t CLASS_NAME[] = L"GameWindow";
	WNDCLASSEXW wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wndClass.lpszClassName = CLASS_NAME;
	if (!RegisterClassExW(&wndClass)) {
		OutputDebugStringW(L"ERROR: ウィンドウ クラスを登録できませんでした。\n");
		return false;
	}

	RECT rect = { 0, 0, ScreenWidth, ScreenHeight };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
	hWnd = CreateWindowExW(
		0, CLASS_NAME, WindowTitle.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
		OutputDebugStringW(L"ERROR: ウィンドウを作成できませんでした。\n");
		return false;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	return true;
}

int Run(const std::wstring& windowTitle, int screenWidth, int screenHeight)
{
	if (!InitWindow(windowTitle, screenWidth, screenHeight)) {
		MessageBoxW(NULL, L"ウィンドウを作成できませんでした。", L"エラー", MB_OK);
		return 0;
	}

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessageW(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return (int)msg.wParam;
}
