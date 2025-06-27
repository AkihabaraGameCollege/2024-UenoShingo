//=============================================================================
// Game.h
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#pragma once

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#pragma comment(lib, "D3D11.lib")
#include <DirectXMath.h>
#include <string>

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

// アプリケーション全体を表します。
class Game
{
public:
	// コンストラクター
	Game(int screenWidth, int screenHeight);

	int Run();

	int GetWidth();
	int GetHeight();

private:
	// ウィンドウの幅
	int screenWidth = 640;
	// ウィンドウの高さ
	int screenHeight = 480;

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

	bool InitGraphicsDevice();
};

/// <summary>
/// ウィンドウを作成してメッセージループを処理します。
/// </summary>
class Application
{
public:
	static int Run(
		Game* game, HINSTANCE hInstance, int nCmdShow, const std::wstring& title);
	static HWND GetWindowHandle();

private:
	static bool InitializeWindow(
		HINSTANCE hInstance, int nCmdShow, const std::wstring& title,
		int width, int height);

	static HWND hWnd;
};
