#include "SampleGame.h"
#include <random>

using namespace GameLibrary;
using namespace DirectX;

namespace
{
	// 矩形（頂点）
	constexpr Vertex_Sprite quadVertices[] = {
		{ { -0.5f, +0.5f, +0.0f, }, { 0.0f, 0.0f, }, },
		{ { +0.5f, +0.5f, +0.0f, }, { 1.0f, 0.0f, }, },
		{ { -0.5f, -0.5f, +0.0f, }, { 0.0f, 1.0f, }, },
		{ { +0.5f, -0.5f, +0.0f, }, { 1.0f, 1.0f, }, },
	};
	// 矩形（インデックス）
	constexpr uint32_t quadIndices[] = {
		0, 1, 2,
		3, 2, 1,
	};

	constexpr Vertex_Base cubeVertices[] = {
		// Front
		{ { +0.5f, +0.5f, +0.5f, }, { +0.0f, +0.0f, +1.0f }, { 0.0f, 0.0f }, },
		{ { -0.5f, +0.5f, +0.5f, }, { +0.0f, +0.0f, +1.0f }, { 1.0f, 0.0f }, },
		{ { +0.5f, -0.5f, +0.5f, }, { +0.0f, +0.0f, +1.0f }, { 0.0f, 1.0f }, },
		{ { -0.5f, -0.5f, +0.5f, }, { +0.0f, +0.0f, +1.0f }, { 1.0f, 1.0f }, },
		// Back
		{ { -0.5f, +0.5f, -0.5f, }, { +0.0f, +0.0f, -1.0f }, { 0.0f, 0.0f }, },
		{ { +0.5f, +0.5f, -0.5f, }, { +0.0f, +0.0f, -1.0f }, { 1.0f, 0.0f }, },
		{ { -0.5f, -0.5f, -0.5f, }, { +0.0f, +0.0f, -1.0f }, { 0.0f, 1.0f }, },
		{ { +0.5f, -0.5f, -0.5f, }, { +0.0f, +0.0f, -1.0f }, { 1.0f, 1.0f }, },
		// Right
		{ { +0.5f, +0.5f, -0.5f, }, { +1.0f, +0.0f, +0.0f }, { 0.0f, 0.0f }, },
		{ { +0.5f, +0.5f, +0.5f, }, { +1.0f, +0.0f, +0.0f }, { 1.0f, 0.0f }, },
		{ { +0.5f, -0.5f, -0.5f, }, { +1.0f, +0.0f, +0.0f }, { 0.0f, 1.0f }, },
		{ { +0.5f, -0.5f, +0.5f, }, { +1.0f, +0.0f, +0.0f }, { 1.0f, 1.0f }, },
		// Left
		{ { -0.5f, +0.5f, +0.5f, }, { -1.0f, +0.0f, +0.0f }, { 0.0f, 0.0f }, },
		{ { -0.5f, +0.5f, -0.5f, }, { -1.0f, +0.0f, +0.0f }, { 1.0f, 0.0f }, },
		{ { -0.5f, -0.5f, +0.5f, }, { -1.0f, +0.0f, +0.0f }, { 0.0f, 1.0f }, },
		{ { -0.5f, -0.5f, -0.5f, }, { -1.0f, +0.0f, +0.0f }, { 1.0f, 1.0f }, },
		// Top
		{ { -0.5f, +0.5f, +0.5f, }, { +0.0f, +1.0f, +0.0f }, { 0.0f, 0.0f }, },
		{ { +0.5f, +0.5f, +0.5f, }, { +0.0f, +1.0f, +0.0f }, { 1.0f, 0.0f }, },
		{ { -0.5f, +0.5f, -0.5f, }, { +0.0f, +1.0f, +0.0f }, { 0.0f, 1.0f }, },
		{ { +0.5f, +0.5f, -0.5f, }, { +0.0f, +1.0f, +0.0f }, { 1.0f, 1.0f }, },
		// Bottom
		{ { +0.5f, -0.5f, +0.5f, }, { +0.0f, -1.0f, +0.0f }, { 0.0f, 0.0f }, },
		{ { -0.5f, -0.5f, +0.5f, }, { +0.0f, -1.0f, +0.0f }, { 1.0f, 0.0f }, },
		{ { +0.5f, -0.5f, -0.5f, }, { +0.0f, -1.0f, +0.0f }, { 0.0f, 1.0f }, },
		{ { -0.5f, -0.5f, -0.5f, }, { +0.0f, -1.0f, +0.0f }, { 1.0f, 1.0f }, },
	};
	constexpr uint32_t cubeIndices[] = {
		// Front
		0, 1, 2, 3, 2, 1,
		// Back
		4, 5, 6, 7, 6, 5,
		// Right
		8, 9, 10, 11, 10, 9,
		// Left
		12, 13, 14, 15, 14, 13,
		// Top
		16, 17, 18, 19, 18, 17,
		// Bottom
		20, 21, 22, 23, 22, 21,
	};

	constexpr uint32_t textureSource[] = {
		0xFFFFFFFF, 0xFFCCCCCC, 0xFFFFFFFF, 0xFFCCCCCC,
		0xFFCCCCCC, 0xFFFFFFFF, 0xFFCCCCCC, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFCCCCCC, 0xFFFFFFFF, 0xFFCCCCCC,
		0xFFCCCCCC, 0xFFFFFFFF, 0xFFCCCCCC, 0xFFFFFFFF,
	};
}

/// <summary>
/// このクラスのインスタンスを初期化します。
/// </summary>
SampleGame::SampleGame(const GameLibrary::ProjectSettings& settings)
	: Game(settings)
{

}

/// <summary>
/// 初期化処理を実装します。
/// </summary>
void SampleGame::OnInitialize()
{
	// 定数バッファー
	constantBufferManager = std::make_shared<ConstantBufferManager>(device.Get());

	// 平行ライト
	XMStoreFloat4(&lightRotation, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(50),
		XMConvertToRadians(-30),
		XMConvertToRadians(0)));
	lightColor = { 1, 1, 1, 1 };

	constantBufferPerLighting = std::make_shared<ConstantBuffer>(device.Get(),
		static_cast<UINT>(sizeof constantsPerLighting));
	constantBufferManager->Add("ConstantBufferPerLighting", constantBufferPerLighting);

	constantBufferPerFrame = std::make_shared<ConstantBuffer>(device.Get(),
		static_cast<UINT>(sizeof constantsPerFrame));
	constantBufferManager->Add("ConstantBufferPerFrame", constantBufferPerFrame);

	constantBufferPerDraw = std::make_shared<ConstantBuffer>(device.Get(),
		static_cast<UINT>(sizeof constantsPerDraw));
	constantBufferManager->Add("ConstantBufferPerDraw", constantBufferPerDraw);

	// --- リソースのロード（再利用するため変数に保持） ---

	// 3D用メッシュ (Cube)
	meshCube = std::make_shared<Mesh>(device.Get());
	meshCube->SetVertexBuffer(Vertex_Base::GetSize(), static_cast<UINT>(std::size(cubeVertices)), cubeVertices);
	meshCube->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	meshCube->SetIndexBuffer(IndexFormat::UInt32, static_cast<UINT>(std::size(cubeIndices)), cubeIndices);

	// Sprite用メッシュ (Quad)
	meshQuad = std::make_shared<Mesh>(device.Get());
	meshQuad->SetVertexBuffer(Vertex_Sprite::GetSize(), static_cast<UINT>(std::size(quadVertices)), quadVertices);
	meshQuad->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	meshQuad->SetIndexBuffer(IndexFormat::UInt32, static_cast<UINT>(std::size(quadIndices)), quadIndices);

	// テクスチャ
	auto mainTexture = std::make_shared<Texture2D>(device.Get(), 4, 4, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, textureSource);

	// マテリアル & シェーダー (3D用)
	{
		auto shader = shaderManager->Find(L"Shader/Standard");
		matPlayer = std::make_shared<Material>(device.Get(), constantBufferManager, shader);
		matPlayer->SetTexture("MainTexture", mainTexture);

		matEnemy = std::make_shared<Material>(device.Get(), constantBufferManager, shader);
		matEnemy->SetTexture("MainTexture", mainTexture);
		matEnemy->SetColor("AlbedoColor", XMFLOAT4{ 1.0f, 0.2f, 0.2f, 1.0f }); // 赤色
	}
	// マテリアル & シェーダー (Sprite用)
	{
		auto shader = shaderManager->Find(L"Shader/Sprite");
		matBullet = std::make_shared<Material>(device.Get(), constantBufferManager, shader);
		matBullet->SetTexture("MainTexture", mainTexture);
		matBullet->SetColor("AlbedoColor", XMFLOAT4{ 1.0f, 1.0f, 0.0f, 1.0f }); // 黄色
	}

	// 入力レイアウト
	inputLayout3D = std::make_shared<InputLayout_Base>(device.Get());
	inputLayoutSprite = std::make_shared<InputLayout_Sprite>(device.Get());

	// --- プレイヤーの初期化 ---
	player = std::make_unique<Player>();
	player->mesh = meshCube;
	player->material = matPlayer;
	player->inputLayout = inputLayout3D;
	player->Initialize();
}

/// <summary>
/// フレームの更新処理を実装します。
/// </summary>
void SampleGame::OnUpdate()
{
	if (isGameOver) {
		// ゲームオーバー時の処理（リセットなど）を入れるならここ
		if (Input::GetButton(DigitalInput::Space)) {
			// リセット処理
			isGameOver = false;
			bullets.clear();
			enemies.clear();
			player->localPosition = { 0, 0, 0 };
		}
		return;
	}

	// 1. プレイヤー更新
	player->Update();

	// 2. 弾の発射（Spaceキー）
	if (shotCooldown > 0) shotCooldown -= Time::GetDeltaTime();
	if (Input::GetButton(DigitalInput::Space) && shotCooldown <= 0) { // 入力設定によってキーを変更してください
		SpawnBullet();
		shotCooldown = 0.2f; // 連射速度
	}

	// 3. 敵の出現（タイマー）
	enemySpawnTimer += Time::GetDeltaTime();
	if (enemySpawnTimer > 2.0f) { // 2秒ごとに生成
		SpawnEnemy();
		enemySpawnTimer = 0.0f;
	}

	// 4. 弾の更新と削除
	for (auto it = bullets.begin(); it != bullets.end(); ) {
		(*it)->Update();
		// 画面外に出たら消す
		if ((*it)->localPosition.y > 15.0f) {
			it = bullets.erase(it);
		}
		else if (!(*it)->isActive) {
			it = bullets.erase(it);
		}
		else {
			++it;
		}
	}

	// 5. 敵の更新と削除
	for (auto it = enemies.begin(); it != enemies.end(); ) {
		// 単純な移動：下に降りてくる
		auto pos = XMLoadFloat3(&(*it)->localPosition);
		pos += XMVectorSet(0, -3.0f, 0, 0) * Time::GetDeltaTime();
		XMStoreFloat3(&(*it)->localPosition, pos);

		(*it)->Update();

		// プレイヤーとの衝突
		if (CheckCollision(*player, **it)) {
			isGameOver = true; // 衝突したらゲームオーバー
		}

		// 画面外に出たら消す
		if ((*it)->localPosition.y < -15.0f) {
			it = enemies.erase(it);
		}
		else if (!(*it)->isActive) {
			it = enemies.erase(it);
		}
		else {
			++it;
		}
	}

	// 6. 弾と敵の衝突判定
	for (auto& bullet : bullets) {
		if (!bullet->isActive) continue;
		for (auto& enemy : enemies) {
			if (!enemy->isActive) continue;

			if (CheckCollision(*bullet, *enemy)) {
				bullet->isActive = false; // 弾消滅
				enemy->isActive = false;  // 敵消滅
				break; // 弾は1つにつき1体の敵のみ倒す
			}
		}
	}
}

/// <summary>
/// 弾を生成します。
/// </summary>
void SampleGame::SpawnBullet()
{
	auto bullet = std::make_shared<Sprite>();
	bullet->mesh = meshQuad;
	bullet->material = matBullet;
	bullet->inputLayout = inputLayoutSprite;
	bullet->Initialize();

	// プレイヤーの位置から少し上に発射
	bullet->localPosition = player->localPosition;
	bullet->localPosition.y += 0.8f;
	bullet->localScale = { 0.5f, 0.5f, 0.5f };
	// 上方向に飛ぶ
	bullet->velocity = { 0, 15.0f, 0 };

	bullets.push_back(bullet);
}

/// <summary>
/// 敵を生成します。
/// </summary>
void SampleGame::SpawnEnemy()
{
	auto enemy = std::make_shared<GameObject>();
	enemy->mesh = meshCube;
	enemy->material = matEnemy;
	enemy->inputLayout = inputLayout3D;
	enemy->Initialize();

	// 画面上部のランダムな位置に出現
	float randX = (float)(rand() % 200 - 100) / 10.0f; // -10 to 10
	enemy->localPosition = { randX, 10.0f, 0 };

	enemies.push_back(enemy);
}

/// <summary>
/// 2つのオブジェクト間の衝突を判定します（円判定）。
/// </summary>
bool SampleGame::CheckCollision(const GameObject& a, const GameObject& b)
{
	XMVECTOR posA = XMLoadFloat3(&a.localPosition);
	XMVECTOR posB = XMLoadFloat3(&b.localPosition);

	// 距離の二乗を計算（高速化のため平方根は取らない）
	XMVECTOR distSqVec = XMVector3LengthSq(posA - posB);
	float distSq;
	XMStoreFloat(&distSq, distSqVec);

	float radiusSum = a.radius + b.radius;
	return distSq < (radiusSum * radiusSum);
}

/// <summary>
/// フレームの描画処理を実装します。
/// </summary>
void SampleGame::OnRender()
{
	// 平行ライト
	const auto lightWorldMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&lightRotation));
	const auto lightForward = lightWorldMatrix.r[2];
	XMStoreFloat4(&constantsPerLighting.LightDirection, lightForward);
	XMStoreFloat4(&constantsPerLighting.LightColor, XMColorSRGBToRGB(XMLoadFloat4(&lightColor)));
	constantBufferPerLighting->UpdateSubresource(&constantsPerLighting);

	// メイン カメラ
	const auto cameraWorldMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&cameraRotation));
	const auto cameraForward = cameraWorldMatrix.r[2];
	const auto cameraUp = cameraWorldMatrix.r[1];
	auto matrixView = XMMatrixLookToLH(XMLoadFloat3(&cameraPosition), cameraForward, cameraUp);
	XMStoreFloat4x4(&constantsPerFrame.MatrixView, XMMatrixTranspose(matrixView));

	// プロジェクション
	const auto aspectRatio = GetWidth() / static_cast<float>(GetHeight());
	auto matrixProjection = XMMatrixIdentity();
	if (orthographic) {
		matrixProjection = XMMatrixOrthographicLH(
			orthographicSize * aspectRatio, orthographicSize, clipPlaneNear, clipPlaneFar);
	}
	else {
		matrixProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(fieldOfView), aspectRatio, clipPlaneNear, clipPlaneFar);
	}
	XMStoreFloat4x4(&constantsPerFrame.MatrixProjection, XMMatrixTranspose(matrixProjection));

	XMStoreFloat4x4(&constantsPerFrame.MatrixViewProjection, XMMatrixTranspose(matrixView * matrixProjection));
	constantBufferPerFrame->UpdateSubresource(&constantsPerFrame);

	// --- 描画実行 ---

	// プレイヤー
	if (!isGameOver) {
		XMStoreFloat4x4(&constantsPerDraw.MatrixWorld, XMMatrixTranspose(player->GetLocalToWorldMatrix()));
		constantBufferPerDraw->UpdateSubresource(&constantsPerDraw);
		player->Render(deviceContext.Get());
	}

	// 敵
	for (const auto& enemy : enemies) {
		XMStoreFloat4x4(&constantsPerDraw.MatrixWorld, XMMatrixTranspose(enemy->GetLocalToWorldMatrix()));
		constantBufferPerDraw->UpdateSubresource(&constantsPerDraw);
		enemy->Render(deviceContext.Get());
	}

	// 弾 (Sprite)
	for (const auto& bullet : bullets) {
		XMStoreFloat4x4(&constantsPerDraw.MatrixWorld, XMMatrixTranspose(bullet->GetLocalToWorldMatrix()));
		constantBufferPerDraw->UpdateSubresource(&constantsPerDraw);
		bullet->Render(deviceContext.Get());
	}
}