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
	std::shared_ptr<GameLibrary::ConstantBufferManager> constantBufferManager;

	// ライト
	DirectX::XMFLOAT4 lightRotation = { 0, 0, 0, 1 };
	DirectX::XMFLOAT4 lightColor = { 1, 1, 1, 1 };

	struct ConstantsPerLighting
	{
		DirectX::XMFLOAT4 LightDirection;
		DirectX::XMFLOAT4 LightColor;
	};
	ConstantsPerLighting constantsPerLighting = {};
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerLighting;

	// メイン カメラ
	DirectX::XMFLOAT3 cameraPosition = { 0, 1, -10 };
	DirectX::XMFLOAT4 cameraRotation = { 0, 0, 0, 1 };
	// プロジェクション
	float fieldOfView = 60.0f;
	float clipPlaneNear = 0.3f;
	float clipPlaneFar = 1000;
	bool orthographic = false;
	float orthographicSize = 10;

	struct ConstantsPerFrame
	{
		DirectX::XMFLOAT4X4 MatrixView;
		DirectX::XMFLOAT4X4 MatrixProjection;
		DirectX::XMFLOAT4X4 MatrixViewProjection;
	};
	ConstantsPerFrame constantsPerFrame = {};
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerFrame;

	// オブジェクト
	DirectX::XMVECTOR localScale = { 1, 1, 1, };
	DirectX::XMVECTOR localRotation = DirectX::XMQuaternionIdentity();
	DirectX::XMVECTOR localPosition = { 0, 0, 0, };
	DirectX::XMFLOAT4 albedoColor = { 1, 1, 1, 1 };

	std::shared_ptr<GameLibrary::VertexBuffer> vertexBuffer;
	UINT vertexOffset = 0;
	std::shared_ptr<GameLibrary::IndexBuffer> indexBuffer;
	UINT indexOffset = 0;

	struct ConstantsPerDraw
	{
		DirectX::XMFLOAT4X4 MatrixWorld;
	};
	ConstantsPerDraw constantsPerDraw = {};
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerDraw;

	// Shaders
	std::shared_ptr<GameLibrary::Shader> shader;
	// Main Texture
	std::shared_ptr<GameLibrary::Texture2D> mainTexture;
	// マテリアル
	struct ConstantsPerMaterial
	{
		DirectX::XMFLOAT4 Albedo;
	};
	ConstantsPerMaterial constantsPerMaterial = {};
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerMaterial;
	GameLibrary::ConstantBufferMap constantBufferMap;
	GameLibrary::ShaderResourceViewMap shaderResourceViewMap;
	GameLibrary::SamplerStateMap samplerStateMap;

	std::shared_ptr<GameLibrary::InputLayout> inputLayout;

	UINT startIndexLocation = 0;
	INT baseVertexLocation = 0;

	float time = 0;
};
