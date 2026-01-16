#pragma once

#include <GameLibrary.h>
#include <vector>
#include "Player.h"
#include "Sprite.h"

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
	// 弾の発射と敵の生成
	void SpawnBullet();
	void SpawnEnemy();
	// 衝突判定
	bool CheckCollision(const GameObject& a, const GameObject& b);

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
	DirectX::XMFLOAT3 cameraPosition = { 0, 0, -15 }; // カメラ位置調整
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

	// --- ゲームオブジェクト管理 ---
	std::unique_ptr<Player> player;

	// 弾と敵のリスト
	std::vector<std::shared_ptr<Sprite>> bullets;
	std::vector<std::shared_ptr<GameObject>> enemies;

	// リソース共有
	std::shared_ptr<GameLibrary::Mesh> meshCube;
	std::shared_ptr<GameLibrary::Mesh> meshQuad;
	std::shared_ptr<GameLibrary::Material> matPlayer;
	std::shared_ptr<GameLibrary::Material> matEnemy;
	std::shared_ptr<GameLibrary::Material> matBullet;
	std::shared_ptr<GameLibrary::InputLayout> inputLayout3D;
	std::shared_ptr<GameLibrary::InputLayout> inputLayoutSprite;

	// ゲーム進行管理
	float enemySpawnTimer = 0.0f;
	float shotCooldown = 0.0f;
	bool isGameOver = false;

	struct ConstantsPerDraw
	{
		DirectX::XMFLOAT4X4 MatrixWorld;
	};
	ConstantsPerDraw constantsPerDraw = {};
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerDraw;
};