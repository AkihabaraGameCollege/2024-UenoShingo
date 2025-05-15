//=============================================================================
// Game.cpp
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#include "Game.h"

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

// ウィンドウを作成します。
bool InitWindow(Game* game)
{
	const auto hInstance = GetModuleHandleW(NULL);

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
	RECT rect = { 0, 0, game->ScreenWidth, game->ScreenHeight };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
	// ウィンドウを作成する
	game->hWnd = CreateWindowExW(
		0, CLASS_NAME, game->WindowTitle.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, hInstance, NULL);
	if (game->hWnd == NULL) {
		OutputDebugStringW(L"ERROR: ウィンドウを作成できませんでした。\n");
		return false;
	}

	// ウィンドウの表示指示を出すためにウィンドウ ハンドルを指定する
	ShowWindow(game->hWnd, SW_SHOWNORMAL);
	UpdateWindow(game->hWnd);

	return true;
}

// アプリケーションのエントリーポイントです。
// ウィンドウの作成からメッセージのループ処理を開始します。
int Run(Game* game)
{
	// メインウィンドウを作成
	if (!InitWindow(game)) {
		MessageBoxW(NULL, L"ウィンドウを作成できませんでした。", L"エラー", MB_OK);
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

	return (int)msg.wParam;
}