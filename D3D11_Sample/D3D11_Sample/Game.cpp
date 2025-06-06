//=============================================================================
// Game.cpp
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#include "Game.h"
#include "StandardVertexShader.h"
#include "StandardPixelShader.h"

using namespace Microsoft::WRL;

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

// グラフィックデバイスを初期化します。
bool Game::InitGraphicsDevice()
{
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
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
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
		return false;
	}

	// スワップチェーンからバックバッファーを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"バックバッファーを取得できませんでした。", L"エラー", MB_OK);
		return false;
	}
	// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
	hr = graphicsDevice->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"レンダーターゲット ビューを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}
	// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
	hr = graphicsDevice->CreateShaderResourceView(backBuffer.Get(), NULL, &renderTargetResourceView);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"レンダーターゲット リソース ビューを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}
	backBuffer.Reset();

	// テクスチャのフォーマットを設定
	DXGI_FORMAT textureFormat = depthStencilFormat;
	DXGI_FORMAT resourceFormat = depthStencilFormat;
	switch (depthStencilFormat) {
	case DXGI_FORMAT_D16_UNORM:
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		resourceFormat = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		//resourceFormat = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		//resourceFormat = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
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
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = graphicsDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"深度ステンシルを作成できませんでした。", L"エラー", MB_OK);
		return false;
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
		return false;
	}
	// 深度ステンシルにシェーダーからアクセスするためのリソース ビューを作成
	D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc = {};
	depthStencilResourceViewDesc.Format = resourceFormat;
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		depthStencilResourceViewDesc.Texture2D.MostDetailedMip = 0;
		depthStencilResourceViewDesc.Texture2D.MipLevels = 1;
	}
	hr = graphicsDevice->CreateShaderResourceView(depthStencil.Get(), &depthStencilResourceViewDesc, &depthStencilResourceView);
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"深度ステンシル リソース ビューを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}
	depthStencil.Reset();


	// ビューポート
	viewport.Width = static_cast<FLOAT>(screenWidth);
	viewport.Height = static_cast<FLOAT>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

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
	// グラフィックデバイスを作成
	if (!InitGraphicsDevice()) {
		MessageBoxW(NULL, L"グラフィックデバイスを初期化できませんでした。", L"メッセージ", MB_OK);
		return 0;
	}


	HRESULT hr = S_OK;

	// 一つの頂点に含まれるデータの型
	struct VertexPositionNormalTexture
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル
		DirectX::XMFLOAT2 texCoord;	// テクスチャUV座標
	};
	// 頂点データの配列
	constexpr VertexPositionNormalTexture vertices[] = {
		{ { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f }, },
		{ {  0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f }, },
		{ {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f }, },
	};
	UINT vertexCount = std::size(vertices);

	// 作成する頂点バッファーについての記述
	constexpr auto bufferDesc = D3D11_BUFFER_DESC{
		.ByteWidth = sizeof vertices,
		.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	// バッファーを作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	hr = graphicsDevice->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);
	if (FAILED(hr) || vertexBuffer == nullptr) {
		OutputDebugStringW(L"頂点バッファーを作成できませんでした。");
		return 0;
	}
	// バッファーにデータを転送
	immediateContext->UpdateSubresource(vertexBuffer.Get(), 0, NULL, vertices, 0, 0);

	// 頂点シェーダーを作成
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	hr = graphicsDevice->CreateVertexShader(
		g_StandardVertexShader, std::size(g_StandardVertexShader),
		NULL,
		&vertexShader);
	if (FAILED(hr) || vertexShader == nullptr) {
		OutputDebugStringW(L"頂点シェーダーを作成できませんでした。");
		return 0;
	}
	// ピクセル シェーダーを作成
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	hr = graphicsDevice->CreatePixelShader(
		g_StandardPixelShader, std::size(g_StandardPixelShader),
		NULL,
		&pixelShader);
	if (FAILED(hr) || pixelShader == nullptr) {
		OutputDebugStringW(L"ピクセル シェーダーを作成できませんでした。");
		return 0;
	}

	// 入力レイアウトを作成
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	hr = graphicsDevice->CreateInputLayout(
		inputElementDescs, std::size(inputElementDescs),
		g_StandardVertexShader, std::size(g_StandardVertexShader),
		&inputLayout);
	if (FAILED(hr)) {
		OutputDebugStringW(L"入力レイアウトを作成できませんでした。");
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

		// レンダーターゲットを設定
		ID3D11RenderTargetView* renderTargetViews[] = { renderTargetView.Get(), };
		immediateContext->OMSetRenderTargets(std::size(renderTargetViews), renderTargetViews, depthStencilView.Get());
		// 画面をクリアー
		immediateContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
		immediateContext->ClearDepthStencilView(depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// ビューポートを設定
		D3D11_VIEWPORT viewports[] = { viewport, };
		immediateContext->RSSetViewports(std::size(viewports), viewports);

		// 頂点バッファーを設定
		ID3D11Buffer* const vertexBuffers[1] = { vertexBuffer.Get(), };
		const UINT strides[1] = { sizeof(VertexPositionNormalTexture), };
		const UINT offsets[1] = { 0, };
		immediateContext->IASetVertexBuffers(0, std::size(vertexBuffers), vertexBuffers, strides, offsets);
		// 頂点バッファーと頂点シェーダーの組合せに対応した入力レイアウトを設定
		immediateContext->IASetInputLayout(inputLayout.Get());
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// シェーダーを設定
		immediateContext->VSSetShader(vertexShader.Get(), NULL, 0);
		immediateContext->PSSetShader(pixelShader.Get(), NULL, 0);

		// メッシュを描画
		immediateContext->Draw(vertexCount, 0);

		// バックバッファーに描画したイメージをディスプレイに表示
		HRESULT hr = S_OK;
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