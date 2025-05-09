#include <Windows.h>

HWND hWnd = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"GameWindow";
	WNDCLASSEXW wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wndClass.lpszClassName = CLASS_NAME;
	if (!RegisterClassExW(&wndClass)) {
		MessageBox(NULL, L"ウィンドウ クラスの登録に失敗しました。", L"エラー", MB_OK);
		return -1;
	}

	RECT rect = { 0, 0, 640, 480 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	hWnd = CreateWindowExW(0,
		CLASS_NAME,
		L"タイトル",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
		MessageBox(NULL, L"ウィンドウの作成に失敗しました。", L"エラー", MB_OK);
		return -1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = {};
	while (true) {
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

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		auto hdc = BeginPaint(hWnd, &ps);
		RECT rect = {};
		rect.left = 200;
		rect.top = 100;
		rect.right = 300;
		rect.bottom = 200;
		FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hWnd, &ps);
	}
	break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
