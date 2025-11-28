//=============================================================================
// Game.h
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#pragma once

#include <GameLibrary/Shader.h>

#include <Windows.h>

#include <memory>
#include <string>
#include <format>

#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <DirectXMath.h>

namespace GameLibrary
{
	struct ProjectSettings
	{
		std::wstring Version = L"1.0.0";
		std::wstring Title = L"Game Title";
		int Width = 640;
		int Height = 480;
		bool ForceVSync = false;
		bool UseWarpAdapter = false;
	};

	// アプリケーション全体を表します。
	class Game
	{
	public:
		Game(const ProjectSettings& settings);
		Game(const Game&) noexcept = delete;
		virtual ~Game() = default;

		void Initialize(HWND hWnd);
		void Update() noexcept;
		void Render() noexcept;
		void Release() noexcept;

		const std::wstring& GetTitle() const;
		int GetWidth() const;
		int GetHeight() const;

	private:
		std::wstring title = L"Game Title";
		int width = 640;
		int height = 480;
		bool forceVSync = false;
		bool useWarpAdapter = false;

	protected:
		virtual void OnInitialize() {};
		virtual void OnRelease() {};
		virtual void OnUpdate() {};
		virtual void OnRender() {};

		Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
		bool allowTearing = true;
		Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgiAdapter;
		Microsoft::WRL::ComPtr<IDXGIDevice4> dxgiDevice;

		Microsoft::WRL::ComPtr<ID3D11Device5> device;
		D3D_FEATURE_LEVEL featureLevel = {};
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4> deviceContext;

		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView1> renderTargetView;
		DirectX::XMVECTORF32 clearColor = { 0 / 255.0f, 99 / 255.0f, 181 / 255.0f, 1.0f };
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> depthShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> stencilShaderResourceView;

		D3D11_VIEWPORT viewport = {
			.TopLeftX = 0,
			.TopLeftY = 0,
			.Width = 0,
			.Height = 0,
			.MinDepth = 0.0f,
			.MaxDepth = 1.0f,
		};

		std::unique_ptr<VertexShader> spriteVertexShader;
		std::unique_ptr<PixelShader> spritePixelShader;
	};

	/// <summary>
	/// ウィンドウを作成してメッセージループを処理します。
	/// </summary>
	class Application
	{
	public:
		Application() noexcept = delete;
		Application(const Application&) noexcept = delete;
		~Application() = default;

		static int Run(Game* game, HINSTANCE hInstance, int nCmdShow) noexcept;
		static HWND GetWindowHandle() noexcept;
	};
}
