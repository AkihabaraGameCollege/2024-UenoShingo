#pragma once

#include "GameObject.h"

/// <summary>
/// スプライトのエンティティを表します。
/// </summary>
class Sprite : public GameObject
{
public:
	void Initialize() override;
	void Update() noexcept override;

	// 移動速度
	DirectX::XMFLOAT3 velocity = { 0, 0, 0 };
};