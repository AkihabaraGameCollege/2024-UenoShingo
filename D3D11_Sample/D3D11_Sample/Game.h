//=============================================================================
// Game.h
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#pragma once

#include <Windows.h>

#include <system_error>
#include <source_location>
#include <string>
#include <format>

#include <comdef.h>
#include <wrl/client.h>
#include <d3d11.h>
#pragma comment(lib, "D3D11.lib")
#include <DirectXMath.h>

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

// アプリケーション全体を表します。
class Game
{
public:
	// コンストラクター
	Game(int screenWidth, int screenHeight);

	int GetWidth();
	int GetHeight();

	void Initialize(HWND hWnd);
	void Release();
	void Update();
	void Render();

protected:
	virtual void OnInitialize() {};
	virtual void OnRelease() {};
	virtual void OnUpdate() {};
	virtual void OnRender() {};

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

private:
	void InitGraphicsDevice(HWND hWnd);
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
	static void InitializeWindow(
		Game* game,
		HINSTANCE hInstance, int nCmdShow, const std::wstring& title,
		int width, int height);

	static HWND hWnd;
};

/// <summary>
/// 直前のシステムエラーを例外としてスローします。
/// </summary>
inline void ThrowLastError(const std::source_location& location = std::source_location::current())
{
	const auto errorCode = GetLastError();

	const auto message = std::format(
		"{0}({1},{2}): error {3}",
		location.file_name(), location.line(), location.column(), errorCode);
	OutputDebugStringA(message.c_str());
	OutputDebugStringA(": ");

	LPSTR buffer = nullptr;
	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, LANG_USER_DEFAULT,
		reinterpret_cast<LPSTR>(&buffer), 0, NULL);
	OutputDebugStringA(buffer);
	LocalFree(buffer);

	throw std::system_error(
		std::error_code(errorCode, std::system_category()),
		message);
}

/// <summary>
/// 直前のシステムエラーを例外としてスローします。
/// </summary>
inline void ThrowIfFailed(HRESULT hr, const std::source_location& location = std::source_location::current())
{
	if (SUCCEEDED(hr)) {
		return;
	}

	const auto error = _com_error(hr);

	char message[512] = {};
	sprintf_s(
		message, std::size(message),
		"%s(%d,%d): error 0x%X",
		location.file_name(), location.line(), location.column(), hr);
	OutputDebugStringA(message);
	OutputDebugStringA(": ");

	OutputDebugString(error.ErrorMessage());
	OutputDebugString(TEXT("\n"));

	throw error;
}
