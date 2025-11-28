#pragma once

#include <GameLibrary.h>

/// <summary>
/// サンプルのゲームを表します。
/// </summary>
class SampleGame : public GameLibrary::Game
{
public:
	SampleGame(const GameLibrary::ProjectSettings& settings);

protected:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnRender() override;

private:
	std::shared_ptr<GameLibrary::VertexBuffer> vertexBuffer;
	UINT vertexOffset = 0;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	UINT indexCount = 0;
	// Shaders
	GameLibrary::VertexShader* vertexShader = nullptr;
	GameLibrary::GeometryShader* geometryShader = nullptr;
	GameLibrary::PixelShader* pixelShader = nullptr;

	std::shared_ptr<GameLibrary::InputLayout> inputLayout;

	UINT startVertexLocation = 0;

	// 定数バッファーを介してシェーダーに毎フレーム送るデータを表します。
	struct ConstantBufferPerFrame
	{
		DirectX::XMFLOAT4X4 worldMatrix;		// ワールド変換行列
		DirectX::XMFLOAT4X4 viewMatrix;			// ビュー変換行列
		DirectX::XMFLOAT4X4 projectionMatrix;	// プロジェクション変換行列
		DirectX::XMFLOAT4X4 wvpMatrix;			// ワールド × ビュー × プロジェクション変換行列
		// カメラの位置座標
		DirectX::XMFLOAT4 viewPosition = DirectX::XMFLOAT4(0, 1, -10, 1);
		// ライトの位置座標(平行光源 w = 0, 点光源 w = 1)
		DirectX::XMFLOAT4 lightPosition = DirectX::XMFLOAT4(1.0f, 2.0f, -2.0f, 1.0f);
		// マテリアルの表面カラー
		DirectX::XMFLOAT4 materialDiffuseColor = DirectX::XMFLOAT4(1, 1, 0, 1);
		// 鏡面反射の色(r, g, b) = (x, y, z)
		DirectX::XMFLOAT3 materialSpecularColor = DirectX::XMFLOAT3(1, 1, 1);
		// 鏡面反射の強さ(float) = w
		float materialSpecularPower = 1;
	};
	ConstantBufferPerFrame constantBufferPerFrame = {};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	float time = 0;
};
