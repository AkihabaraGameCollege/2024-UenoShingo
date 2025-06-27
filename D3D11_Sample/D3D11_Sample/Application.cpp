#include "Game.h"

namespace
{
	// ウィンドウ・メッセージを処理するプロシージャー
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_CLOSE:
			// ウィンドウを閉じる
			if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"情報", MB_OKCANCEL) == IDOK) {
				DestroyWindow(hWnd);
			}
			return 0;

		case WM_DESTROY:
			// アプリケーションを終了
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

// ウィンドウのハンドル
HWND Application::hWnd = NULL;

/// <summary>
/// ウィンドウを作成してメッセージループを開始します。
/// </summary>
/// <param name="hInstance">インスタンスのハンドル</param>
/// <param name="nCmdShow">ウィンドウの表示方法</param>
/// <param name="title">ウィンドウのタイトル</param>
/// <param name="width">クライアント領域の幅</param>
/// <param name="height">クライアント領域の高さ</param>
/// <returns>アプリケーションの終了コード</returns>
int Application::Run(
	Game* game, HINSTANCE hInstance, int nCmdShow, const std::wstring& title)
{
	if (!InitializeWindow(hInstance, nCmdShow, title, game->GetWidth(), game->GetHeight())) {
		OutputDebugStringW(L"ERROR: ウィンドウを作成できませんでした。\n");
		return 0;
	}

	// メッセージループを実行
	MSG msg = {};
	while (msg.message != WM_QUIT) {
		// このウィンドウのメッセージが存在するかを確認
		if (PeekMessageW(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessageW(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

/// <summary>
/// ウィンドウのハンドルを取得します。
/// </summary>
/// <returns>ウィンドウ ハンドル</returns>
HWND Application::GetWindowHandle()
{
	return hWnd;
}

/// <summary>
/// ウィンドウを初期化します。
/// </summary>
/// <param name="hInstance">インスタンスのハンドル</param>
/// <param name="nCmdShow">ウィンドウの表示方法</param>
/// <param name="title">ウィンドウのタイトル</param>
/// <param name="width">クライアント領域の幅</param>
/// <param name="height">クライアント領域の高さ</param>
/// <returns>関数の実行が成功した場合は true、失敗した場合は false</returns>
bool Application::InitializeWindow(
	HINSTANCE hInstance, int nCmdShow, const std::wstring& title,
	int width, int height)
{
	// ウィンドウ クラスを登録する
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

	// ウィンドウサイズを計算
	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
	// ウィンドウを作成する
	hWnd = CreateWindowExW(
		0, CLASS_NAME, title.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
		OutputDebugStringW(L"ERROR: ウィンドウを作成できませんでした。\n");
		return false;
	}

	// ウィンドウの表示指示を出すためにウィンドウ ハンドルを指定する
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}
