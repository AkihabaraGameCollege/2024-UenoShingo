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

// アプリケーション全体を表します。
class Game
{
public:
	// コンストラクター
	Game(const std::wstring& windowTitle, int screenWidth, int screenHeight);

	int Run();

private:
	// ウィンドウのタイトル
	std::wstring windowTitle = L"タイトル";
	// ウィンドウの幅
	int screenWidth = 640;
	// ウィンドウの高さ
	int screenHeight = 480;
	// ウィンドウのハンドル
	HWND hWnd = NULL;

	bool InitWindow();

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
