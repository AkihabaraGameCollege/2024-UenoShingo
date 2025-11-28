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
	// オブジェクト
	DirectX::XMFLOAT4 albedoColor = { 1, 1, 1, 1 };

	std::shared_ptr<GameLibrary::VertexBuffer> vertexBuffer;
	UINT vertexOffset = 0;
	std::shared_ptr<GameLibrary::IndexBuffer> indexBuffer;
	UINT indexOffset = 0;
	// Shaders
	GameLibrary::VertexShader* vertexShader = nullptr;
	GameLibrary::GeometryShader* geometryShader = nullptr;
	GameLibrary::PixelShader* pixelShader = nullptr;
	// マテリアル
	struct ConstantsPerMaterial
	{
		DirectX::XMFLOAT4 Albedo;
	};
	ConstantsPerMaterial constantsPerMaterial = {};
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerMaterial;

	std::shared_ptr<GameLibrary::InputLayout> inputLayout;

	UINT startIndexLocation = 0;
	INT baseVertexLocation = 0;

	float time = 0;
};
