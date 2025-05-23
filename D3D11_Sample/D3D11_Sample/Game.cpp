//=============================================================================
// Game.cpp
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#include "Game.h"
#include <wrl/client.h>
#include <d3d11.h>
#pragma comment(lib, "D3D11.lib")

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

// このクラスを初期化します。
Game::Game(const std::wstring& windowTitle, int screenWidth, int screenHeight)
{
	this->windowTitle = windowTitle;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

// ウィンドウを作成します。
bool Game::InitWindow()
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
	RECT rect = { 0, 0, this->screenWidth, this->screenHeight };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
	// ウィンドウを作成する
	this->hWnd = CreateWindowExW(
		0, CLASS_NAME, this->windowTitle.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, hInstance, NULL);
	if (this->hWnd == NULL) {
		OutputDebugStringW(L"ERROR: ウィンドウを作成できませんでした。\n");
		return false;
	}

	// ウィンドウの表示指示を出すためにウィンドウ ハンドルを指定する
	ShowWindow(this->hWnd, SW_SHOWNORMAL);
	UpdateWindow(this->hWnd);

	return true;
}

// アプリケーションのエントリーポイントです。
// ウィンドウの作成からメッセージのループ処理を開始します。
int Game::Run()
{
	// メインウィンドウを作成
	if (!InitWindow()) {
		MessageBoxW(NULL, L"ウィンドウを作成できませんでした。", L"エラー", MB_OK);
		return 0;
	}


	// Direct3D 11のデバイス
	Microsoft::WRL::ComPtr<ID3D11Device> graphicsDevice;
	// Direct3D 11のデバイス コンテキスト
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	// レンダーターゲット
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	// バックバッファーをシェーダーで利用するためのリソース ビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> renderTargetResourceView;
	// 深度ステンシルのフォーマット
	//const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	// 深度ステンシル
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	// 画面クリアーに使用するカラー
	FLOAT clearColor[] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };

	HRESULT hr = S_OK;

	// デバイス作成時のオプションフラグ
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// 作成するスワップチェーンについての記述
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT |
		DXGI_USAGE_SHADER_INPUT;	// シェーダーリソースとして使用することを設定
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Windowed = TRUE;

	// デバイス、デバイスコンテキストを作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, 0,
		creationFlags,
		featureLevels, std::size(featureLevels),
		D3D11_SDK_VERSION,
		&swapChainDesc, &swapChain,
		&graphicsDevice, &featureLevel, &immediateContext);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"Direct3D 11デバイスを作成できませんでした。", L"エラー", MB_OK);
		return 0;
	}

	// スワップチェーンからバックバッファーを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"バックバッファーを取得できませんでした。", L"エラー", MB_OK);
		return 0;
	}
	// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
	hr = graphicsDevice->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"レンダーターゲット ビューを作成できませんでした。", L"エラー", MB_OK);
		return 0;
	}
	// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
	hr = graphicsDevice->CreateShaderResourceView(backBuffer.Get(), NULL, &renderTargetResourceView);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"レンダーターゲット リソース ビューを作成できませんでした。", L"エラー", MB_OK);
		return 0;
	}
	backBuffer.Reset();


	// テクスチャのフォーマットを設定
	DXGI_FORMAT textureFormat = depthStencilFormat;
	switch (depthStencilFormat) {
	case DXGI_FORMAT_D16_UNORM:
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
		break;
	}
	// 深度ステンシルを作成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
	depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = textureFormat;
	depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = graphicsDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"深度ステンシルを作成できませんでした。", L"エラー", MB_OK);
		return 0;
	}
	// 深度ステンシルにアクセスするためのビューを作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencilFormat;
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	}
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = graphicsDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &depthStencilView);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"深度ステンシル ビューを作成できませんでした。", L"エラー", MB_OK);
		return 0;
	}
	depthStencil.Reset();


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

		// レンダーターゲットを設定
		ID3D11RenderTargetView* renderTargetViews[] = { renderTargetView.Get(), };
		immediateContext->OMSetRenderTargets(std::size(renderTargetViews), renderTargetViews, depthStencilView.Get());
		// 画面をクリアー
		immediateContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
		immediateContext->ClearDepthStencilView(depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// Direct3Dによる描画処理

		// バックバッファーに描画したイメージをディスプレイに表示
		hr = swapChain->Present(1, 0);
		if (FAILED(hr)) {
			MessageBoxW(hWnd,
				L"グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。",
				L"エラー", MB_OK);
			return 0;
		}
	}


	return (int)msg.wParam;
}