//=============================================================================
// Game.h
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#pragma once

#include <Windows.h>

#include <string>
#include <format>

#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>

namespace GameLibrary
{
	// 一つの頂点に含まれるデータの型
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT4 color;	// 頂点カラー

		static constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
			{ "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	// 位置座標と法線ベクトルを頂点情報に持つデータを表します。
	struct VertexPositionNormal
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル

		// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
		static constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	// 一つの頂点に含まれるデータの型
	struct VertexPositionNormalTexture
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル
		DirectX::XMFLOAT2 texCoord;	// テクスチャUV座標

		static constexpr D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	struct ProjectSettings
	{
		std::wstring Version = L"1.0.0";
		std::wstring Title = L"Game Title";
		int Width = 640;
		int Height = 480;
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

	protected:
		virtual void OnInitialize() {};
		virtual void OnRelease() {};
		virtual void OnUpdate() {};
		virtual void OnRender() {};

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
		// 深度ステンシルをシェーダーで利用するためのリソース ビュー
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthStencilResourceView;
		// 画面クリアーに使用するカラー
		FLOAT clearColor[4] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
		// ビューポート
		D3D11_VIEWPORT viewport = {};

	private:
		void InitGraphicsDevice(HWND hWnd);
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
