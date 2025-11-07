#include <GameLibrary/Game.h>
#include <GameLibrary/Time.h>
#include <GameLibrary/Input.h>
#include <GameLibrary/Utility.h>
#include <system_error>
#include <format>

using namespace GameLibrary;

namespace
{
	ATOM s_ClassAtom = 0;
	HWND s_hWnd = NULL;

	/// <summary>
	/// ウィンドウ メッセージを処理します。
	/// </summary>
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto const game = reinterpret_cast<Game*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

		switch (msg) {
		case WM_CREATE: {
			const auto createStruct = reinterpret_cast<LPCREATESTRUCTW>(lParam);
			SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
			return 0;
		}
		case WM_CLOSE:
			if (MessageBoxW(hWnd, L"保存していないデータは破棄されます。", L"ゲームを終了しますか？", MB_YESNO) == IDYES) {
				DestroyWindow(hWnd);
				s_hWnd = NULL;
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	void InitializeWindow(Game* game, HINSTANCE hInstance, int nCmdShow)
	{
		// ウィンドウクラスの登録
		const auto wndClass = WNDCLASSEXW{
			.cbSize = sizeof(WNDCLASSEXW),
			.style = 0,
			.lpfnWndProc = WindowProc,
			.cbClsExtra = 0, .cbWndExtra = 0,
			.hInstance = hInstance,
			.hIcon = NULL, .hCursor = LoadCursor(NULL, IDC_ARROW), .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND),
			.lpszMenuName = NULL,
			.lpszClassName = game->GetTitle().c_str(),
		};
		s_ClassAtom = RegisterClassExW(&wndClass);
		if (s_ClassAtom == 0) {
			ThrowLastError();
		}
		// クライアント領域を指定してウィンドウサイズを取得
		auto rect = RECT{ 0, 0, game->GetWidth(), game->GetHeight() };
		if (!AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0)) {
			ThrowLastError();
		}
		// ウィンドウを作成
		s_hWnd = CreateWindowExW(
			0, MAKEINTATOM(s_ClassAtom), game->GetTitle().c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,
			NULL, NULL, hInstance, game);
		if (s_hWnd == NULL) {
			ThrowLastError();
			return;	// C6387対策
		}

		game->Initialize(s_hWnd);

		ShowWindow(s_hWnd, nCmdShow);
		//if (!UpdateWindow(hWnd)) {
		//	ThrowLastError();
		//}
	}
}

namespace GameLibrary
{
	/// <summary>
	/// ウィンドウを作成してメッセージループを開始します。
	/// </summary>
	/// <param name="hInstance">インスタンスのハンドル</param>
	/// <param name="nCmdShow">ウィンドウの表示方法</param>
	/// <param name="title">ウィンドウのタイトル</param>
	/// <param name="width">クライアント領域の幅</param>
	/// <param name="height">クライアント領域の高さ</param>
	/// <returns>アプリケーションの終了コード</returns>
	int Application::Run(Game* game, HINSTANCE hInstance, int nCmdShow) noexcept
	{
		try {
			InitializeWindow(game, hInstance, nCmdShow);
			Input::Initialize(hInstance, GetWindowHandle());
			Time::Initialize();
		}
		catch (const std::system_error& error) {
			MessageBoxA(s_hWnd, error.what(), "ERROR: ウィンドウを初期化できませんでした", MB_OK | MB_ICONERROR);
			return 0;
		}
		catch (const _com_error& error) {
			MessageBox(s_hWnd, error.ErrorMessage(), TEXT("ERROR: グラフィックデバイスを初期化できませんでした。"), MB_OK);
			return 0;
		}

		// メッセージループを実行
		MSG msg = {};
		while (msg.message != WM_QUIT) {
			Input::Update();
			Time::Update();

			// このウィンドウのメッセージが存在するかを確認
			if (PeekMessageW(&msg, NULL, 0, 0, PM_NOREMOVE)) {
				// メッセージを取得
				if (!GetMessageW(&msg, NULL, 0, 0)) {
					break;
				}
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}

			game->Update();
			game->Render();
		}

		game->Release();
		Time::Shutdown();

		return static_cast<int>(msg.wParam);
	}

	/// <summary>
	/// ウィンドウのハンドルを取得します。
	/// </summary>
	/// <returns>ウィンドウ ハンドル</returns>
	HWND Application::GetWindowHandle() noexcept
	{
		return s_hWnd;
	}
}
