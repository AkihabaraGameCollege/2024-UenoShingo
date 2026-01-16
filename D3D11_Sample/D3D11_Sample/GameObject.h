#pragma once

#include <GameLibrary.h>
#include <DirectXMath.h>
#include <memory>

/// <summary>
/// アプリケーション内の全てのエンティティとなる基本クラスを表します。
/// </summary>
class GameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameObject() noexcept;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameObject() = default;

	/// <summary>
	/// 初期化処理を行います。
	/// </summary>
	virtual void Initialize() {};

	/// <summary>
	/// 毎フレームの更新処理を行います。
	/// </summary>
	virtual void Update() noexcept {};

	/// <summary>
	/// 描画処理を行います。
	/// </summary>
	/// <param name="deviceContext">Direct3Dのデバイスコンテキスト</param>
	virtual void Render(ID3D11DeviceContext4* deviceContext) const noexcept;

	/// <summary>
	/// ローカル座標からワールド座標への変換行列を取得します。
	/// </summary>
	/// <returns>ローカルからワールドへの変換行列</returns>
	DirectX::XMMATRIX GetLocalToWorldMatrix() const noexcept;

	/// <summary>
	/// 衝突判定用の半径
	/// </summary>
	float radius = 0.5f;

	/// <summary>
	/// 生存フラグ
	/// </summary>
	bool isActive = true;

	/// <summary>
	/// ローカル座標の位置
	/// </summary>
	DirectX::XMFLOAT3 localPosition = { 0, 0, 0, };

	/// <summary>
	/// ローカル座標の回転（クォータニオン）
	/// </summary>
	DirectX::XMFLOAT4 localRotation = { 0, 0, 0, 1, };

	/// <summary>
	/// ローカル座標のスケール
	/// </summary>
	DirectX::XMFLOAT3 localScale = { 1, 1, 1, };

	/// <summary>
	/// メッシュデータ
	/// </summary>
	std::shared_ptr<GameLibrary::Mesh> mesh;

	/// <summary>
	/// マテリアルデータ
	/// </summary>
	std::shared_ptr<GameLibrary::Material> material;

	/// <summary>
	/// 入力レイアウト
	/// </summary>
	std::shared_ptr<GameLibrary::InputLayout> inputLayout;
};