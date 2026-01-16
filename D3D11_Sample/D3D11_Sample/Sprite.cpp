#include "Sprite.h"
#include <GameLibrary.h>

using namespace GameLibrary;
using namespace DirectX;

// Sprite クラスの初期化処理
void Sprite::Initialize()
{
	// material が設定されている場合、マテリアルの色を黒 (RGBA: 0.0, 0.0, 0.0, 1.0) に設定
	if (material)
		material->SetColor("AlbedoColor", XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f });
}

// Sprite クラスの毎フレーム更新処理
void Sprite::Update() noexcept
{
	// 現在のローカル位置を取得
	auto pos = XMLoadFloat3(&localPosition);
	// 現在の速度を取得
	auto vel = XMLoadFloat3(&velocity);
	// 位置に速度を加算 (時間経過を考慮)
	pos += vel * Time::GetDeltaTime();
	// 更新された位置を保存
	XMStoreFloat3(&localPosition, pos);

	// 現在のローカル回転を取得
	auto localRotation = XMLoadFloat4(&this->localRotation);
	// 回転速度を設定 (1秒で360度回転)
	const auto rotationVelocity = XMQuaternionRotationRollPitchYaw(0, 0, XMConvertToRadians(360));
	// 現在の回転に回転速度を適用 (球面線形補間を使用)
	localRotation = XMQuaternionSlerp(localRotation, XMQuaternionMultiply(localRotation, rotationVelocity), Time::GetDeltaTime());
	// 更新された回転を保存
	XMStoreFloat4(&this->localRotation, localRotation);
}