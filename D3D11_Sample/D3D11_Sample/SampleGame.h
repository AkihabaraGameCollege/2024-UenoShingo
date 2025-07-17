#pragma once
#pragma once

#include "Game.h"

/// <summary>
/// サンプルのゲームを表します。
/// </summary>
class SampleGame : public Game
{
public:
	SampleGame(int screenWidth, int screenHeight);

protected:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnRender() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	UINT indexCount = 0;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	// 定数バッファーを介してシェーダーに毎フレーム送るデータを表します。
	struct ConstantBufferPerFrame
	{
		DirectX::XMFLOAT4X4 worldMatrix;		// ワールド変換行列
		DirectX::XMFLOAT4X4 viewMatrix;			// ビュー変換行列
		DirectX::XMFLOAT4X4 projectionMatrix;	// プロジェクション変換行列
		DirectX::XMFLOAT4X4 wvpMatrix;			// ワールド × ビュー × プロジェクション変換行列
		DirectX::XMFLOAT4 materialColor;
	};
	ConstantBufferPerFrame constantBufferPerFrame = {};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	float time = 0;
};
