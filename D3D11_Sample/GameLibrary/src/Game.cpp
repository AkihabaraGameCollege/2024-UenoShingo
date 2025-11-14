//=============================================================================
// Game.cpp
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#include <GameLibrary/Game.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;
using Microsoft::WRL::ComPtr;

namespace
{
#if defined(_DEBUG)
	constexpr UINT creationFlags =
		D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT |
		D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#else
	constexpr UINT creationFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	constexpr D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
	};

	/// <summary>
	/// ハードウェアで実装されているディスプレイ サブシステムを取得します。
	/// </summary>
	/// <param name="factory">IDXGIFactory7 インターフェイス</param>
	/// <returns>ディスプレイ サブシステム</returns>
	ComPtr<IDXGIAdapter4> GetHardwareAdapter(IDXGIFactory7* factory)
	{
		ComPtr<IDXGIAdapter4> adapter;
		for (
			UINT adapterIndex = 0;
			SUCCEEDED(factory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));
			++adapterIndex)
		{
			auto desc = DXGI_ADAPTER_DESC3{};
			ThrowIfFailed(adapter->GetDesc3(&desc));
			if (desc.Flags & DXGI_ADAPTER_FLAG3::DXGI_ADAPTER_FLAG3_SOFTWARE) {
				continue;
			}

			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11DeviceContext> deviceContext;
			auto featureLevel = D3D_FEATURE_LEVEL{};
			if (SUCCEEDED(D3D11CreateDevice(
				adapter.Get(), D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, NULL,
				creationFlags, featureLevels, std::size(featureLevels),
				D3D11_SDK_VERSION, &device, &featureLevel, &deviceContext)))
			{
				break;
			}
		}
		return adapter;
	}
}

// このクラスを初期化します。
Game::Game(const ProjectSettings& settings)
	: title(settings.Title), width(settings.Width), height(settings.Height)
{

}

// ゲームを初期化します。
void Game::Initialize(HWND hWnd)
{
	// DXGI ファクトリー
#if defined(_DEBUG)
	constexpr UINT factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#else
	constexpr UINT factoryFlags = 0;
#endif
	ThrowIfFailed(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&dxgiFactory)));

	// HARDWARE アダプター
	dxgiAdapter = GetHardwareAdapter(dxgiFactory.Get());
	// WARP アダプター
	if (dxgiAdapter == nullptr) {
		ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter)));
	}

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
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
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
	ThrowIfFailed(hr);

	// スワップチェーンからバックバッファーを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	ThrowIfFailed(hr);
	// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
	hr = graphicsDevice->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView);
	ThrowIfFailed(hr);
	// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
	hr = graphicsDevice->CreateShaderResourceView(backBuffer.Get(), NULL, &renderTargetResourceView);
	ThrowIfFailed(hr);
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
	ThrowIfFailed(hr);
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
	ThrowIfFailed(hr);
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
	ThrowIfFailed(hr);
	depthStencil.Reset();

	viewport = {
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = static_cast<FLOAT>(swapChainDesc.BufferDesc.Width),
		.Height = static_cast<FLOAT>(swapChainDesc.BufferDesc.Height),
		.MinDepth = D3D11_MIN_DEPTH,
		.MaxDepth = D3D11_MAX_DEPTH,
	};

	OnInitialize();
}

// リソースを解放します。
void Game::Release() noexcept
{
	OnRelease();
}

// フレームの更新処理を実行します。
void Game::Update() noexcept
{
	OnUpdate();
}

// フレームの描画処理を実行します。
void Game::Render() noexcept
{
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

	OnRender();

	// バックバッファーに描画したイメージをディスプレイに表示
	HRESULT hr = S_OK;
	hr = swapChain->Present(1, 0);
	if (FAILED(hr)) {
		MessageBoxW(Application::GetWindowHandle(),
			L"グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。",
			L"エラー", MB_OK);
		return;
	}
}

const std::wstring& Game::GetTitle() const
{
	return title;
}

/// <summary>
/// ウィンドウの幅を取得します。
/// </summary>
/// <returns>ウィンドウの幅</returns>
int Game::GetWidth() const
{
	return width;
}

/// <summary>
/// ウィンドウの高さを取得します。
/// </summary>
/// <returns>ウィンドウの高さ</returns>
int Game::GetHeight() const
{
	return height;
}
