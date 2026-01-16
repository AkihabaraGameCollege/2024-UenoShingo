#include "Player.h"
#include <GameLibrary.h>
#include <algorithm> // for clamp

using namespace GameLibrary;
using namespace DirectX;

// プレイヤーの初期化処理
void Player::Initialize()
{
    // プレイヤーのアルベドカラーを緑色に設定
    albedoColor = { 0.0f, 1.0f, 0.0f, 1.0f };
    if (material)
        material->SetColor("AlbedoColor", albedoColor); // マテリアルに色を設定

    // プレイヤーの移動速度と半径を初期化
    speed = 10.0f;
    radius = 0.5f;
}

// プレイヤーの更新処理
void Player::Update() noexcept
{
    auto moveInput = XMFLOAT2{}; // 移動入力の初期化

    // 左右の移動入力を取得
    if (Input::GetButton(DigitalInput::A)) {
        moveInput.x = -1; // 左方向
    }
    else if (Input::GetButton(DigitalInput::D)) {
        moveInput.x = 1; // 右方向
    }

    // 上下の移動入力を取得
    if (Input::GetButton(DigitalInput::W)) {
        moveInput.y = 1; // 上方向
    }
    else if (Input::GetButton(DigitalInput::S)) {
        moveInput.y = -1; // 下方向
    }

    // 入力に基づく速度ベクトルを計算
    auto velocity = XMLoadFloat2(&moveInput);
    if (XMVector2NotEqual(velocity, XMVectorZero())) // 入力がゼロでない場合
    {
        velocity = XMVector2Normalize(velocity); // 正規化して方向を取得
        velocity *= speed; // 速度を掛けて移動量を計算

        // 現在の位置に移動量を加算
        auto pos = XMLoadFloat3(&this->localPosition);
        pos += XMVectorSet(XMVectorGetX(velocity), XMVectorGetY(velocity), 0, 0) * Time::GetDeltaTime();
        XMStoreFloat3(&this->localPosition, pos); // 新しい位置を保存
    }

    // プレイヤーの位置を画面内に制限
    localPosition.x = std::clamp(localPosition.x, -xLimit, xLimit);
    localPosition.y = std::clamp(localPosition.y, -yLimit, yLimit);
}