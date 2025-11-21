//=============================================================================
// Game.cpp
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#include <GameLibrary/Game.h>
#include <GameLibrary/Utility.h>
#include "SpriteVertexShader.h"

using namespace GameLibrary;
using namespace DirectX;
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
	, forceVSync(settings.ForceVSync), useWarpAdapter(settings.UseWarpAdapter)
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
	if (forceVSync) {
		allowTearing = false;
	}
	else {
		BOOL featureSupportData = FALSE;
		ThrowIfFailed(dxgiFactory->CheckFeatureSupport(
			DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING,
			&featureSupportData, sizeof featureSupportData));
		allowTearing = featureSupportData;
	}

	// DXGI アダプター
	if (useWarpAdapter) {
		ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter)));
	}
	else {
		// HARDWARE アダプター
		dxgiAdapter = GetHardwareAdapter(dxgiFactory.Get());
		// WARP アダプター
		if (dxgiAdapter == nullptr) {
			ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter)));
		}
	}
	auto adapterDesc = DXGI_ADAPTER_DESC3{};
	ThrowIfFailed(dxgiAdapter->GetDesc3(&adapterDesc));

	// D3D11/DXGI デバイス
	ComPtr<ID3D11Device> currentDevice;
	ComPtr<ID3D11DeviceContext> currentDeviceContext;
	ThrowIfFailed(D3D11CreateDevice(
		dxgiAdapter.Get(), D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, NULL,
		creationFlags, featureLevels, std::size(featureLevels),
		D3D11_SDK_VERSION, &currentDevice, &featureLevel, &currentDeviceContext));
	ThrowIfFailed(currentDevice.As(&device));
	ThrowIfFailed(currentDevice.As(&dxgiDevice));
	currentDevice.Reset();
	ThrowIfFailed(currentDeviceContext.As(&deviceContext));
	currentDeviceContext.Reset();

	// スワップ チェーン
	ComPtr<IDXGISwapChain1> currentSwapChain;
	auto swapChainDesc = DXGI_SWAP_CHAIN_DESC1{
		.Width = static_cast<UINT>(width),
		.Height = static_cast<UINT>(height),

		//.Format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT,
		//.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
		.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
		//.Format = DXGI_FORMAT::DXGI_FORMAT_R10G10B10A2_UNORM,

		.Stereo = FALSE,
		.SampleDesc = {.Count = 1, .Quality = 0 },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,

		//.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH,
		.Scaling = DXGI_SCALING::DXGI_SCALING_NONE,
		//.Scaling = DXGI_SCALING::DXGI_SCALING_ASPECT_RATIO_STRETCH,

		.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH,
	};
	if (allowTearing) {
		swapChainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}
	const auto fullscreenDesc = DXGI_SWAP_CHAIN_FULLSCREEN_DESC{
		.RefreshRate = {.Numerator = 0, .Denominator = 0, },
		.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED,
		.Windowed = TRUE,
	};
	ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
		device.Get(), hWnd,
		&swapChainDesc, &fullscreenDesc, NULL,
		&currentSwapChain));
	ThrowIfFailed(currentSwapChain.As(&swapChain));

	// バック バッファー
	ComPtr<ID3D11Texture2D1> backBuffer;
	ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	auto backBufferDesc = D3D11_TEXTURE2D_DESC1{};
	backBuffer->GetDesc1(&backBufferDesc);
	// レンダー ターゲット
	auto format = DXGI_FORMAT{};
	switch (backBufferDesc.Format) {
	case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
		format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
		format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT::DXGI_FORMAT_R10G10B10A2_UNORM:
	default:
		format = backBufferDesc.Format;
		break;
	}
	const auto renderTargetViewDesc = D3D11_RENDER_TARGET_VIEW_DESC1{
		.Format = format,
		.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D,
		.Texture2D = {.MipSlice = 0, .PlaneSlice = 0, },
	};
	ThrowIfFailed(device->CreateRenderTargetView1(backBuffer.Get(), &renderTargetViewDesc, &renderTargetView));
	backBuffer.Reset();


	// 深度ステンシル
	ComPtr<ID3D11Texture2D1> depthStencilBuffer;
	const auto depthStencilDesc = D3D11_TEXTURE2D_DESC1{
		.Width = swapChainDesc.Width,
		.Height = swapChainDesc.Height,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS,
		.SampleDesc = swapChainDesc.SampleDesc,
		.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.TextureLayout = D3D11_TEXTURE_LAYOUT::D3D11_TEXTURE_LAYOUT_UNDEFINED,
	};
	ThrowIfFailed(device->CreateTexture2D1(&depthStencilDesc, NULL, &depthStencilBuffer));
	const auto depthStencilViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC{
		.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
		.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D,
		.Flags = 0,
		.Texture2D = {
			.MipSlice = 0, },
	};
	ThrowIfFailed(device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, &depthStencilView));
	ComPtr<ID3D11ShaderResourceView1> depthShaderResourceView;
	auto depthSRV_Desc = D3D11_SHADER_RESOURCE_VIEW_DESC1{
		.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
		.ViewDimension = D3D11_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURE2D,
		.Texture2D = {
			.MostDetailedMip = 0,
			.MipLevels = 1,
			.PlaneSlice = 0, }
	};
	ThrowIfFailed(device->CreateShaderResourceView1(depthStencilBuffer.Get(), &depthSRV_Desc, &depthShaderResourceView));
	ComPtr<ID3D11ShaderResourceView1> stencilShaderResourceView;
	auto stencilSRV_Desc = D3D11_SHADER_RESOURCE_VIEW_DESC1{
		.Format = DXGI_FORMAT::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
		.ViewDimension = D3D11_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURE2D,
		.Texture2D = {
			.MostDetailedMip = 0,
			.MipLevels = 1,
			.PlaneSlice = 0, }
	};
	ThrowIfFailed(device->CreateShaderResourceView1(depthStencilBuffer.Get(), &stencilSRV_Desc, &stencilShaderResourceView));
	depthStencilBuffer.Reset();

	viewport = {
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = static_cast<FLOAT>(swapChainDesc.Width),
		.Height = static_cast<FLOAT>(swapChainDesc.Height),
		.MinDepth = D3D11_MIN_DEPTH,
		.MaxDepth = D3D11_MAX_DEPTH,
	};

	spriteVertexShader = std::make_unique<VertexShader>(
		device.Get(), g_SpriteVertexShader, sizeof g_SpriteVertexShader);

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
	ID3D11RenderTargetView* renderTargetViews[] = { renderTargetView.Get(), };
	deviceContext->OMSetRenderTargets(std::size(renderTargetViews), renderTargetViews, depthStencilView.Get());
	// 画面をクリア
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), XMColorSRGBToRGB(clearColor).m128_f32);
	//deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
	deviceContext->ClearDepthStencilView(
		depthStencilView.Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
	D3D11_VIEWPORT viewports[] = { viewport, };
	deviceContext->RSSetViewports(std::size(viewports), viewports);

	OnRender();

	UINT presentFlags = 0;
	if (allowTearing) {
		presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
	}
	const auto presentParameters = DXGI_PRESENT_PARAMETERS{
		.DirtyRectsCount = 0,
		.pDirtyRects = nullptr,
		.pScrollRect = nullptr,
		.pScrollOffset = nullptr,
	};
	const auto hr = swapChain->Present1(0, presentFlags, &presentParameters);
	if (FAILED(hr)) {
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
