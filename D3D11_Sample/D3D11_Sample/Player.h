#pragma once

#include "GameObject.h"

/// <summary>
/// プレイヤーのエンティティを表します。
/// </summary>
class Player : public GameObject
{
public:
	void Initialize() override;
	void Update() noexcept override;

private:
	DirectX::XMFLOAT4 albedoColor = { 1, 1, 1, 1 };
	float speed = 10.0f; // 速度アップ

	// 移動範囲制限
	const float xLimit = 14.0f;
	const float yLimit = 8.0f;
};