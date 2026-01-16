#include "GameObject.h"

using namespace GameLibrary;
using namespace DirectX;

// GameObject クラスのコンストラクタ
GameObject::GameObject() noexcept
{
}

// ローカル座標からワールド座標への変換行列を取得する関数
XMMATRIX GameObject::GetLocalToWorldMatrix() const noexcept
{
	return XMMatrixTransformation(
		XMVectorZero(), // スケールの中心 (原点)
		XMQuaternionIdentity(), // スケールの回転 (単位クォータニオン)
		XMLoadFloat3(&localScale), // スケール値
		XMVectorZero(), // 回転の中心 (原点)
		XMLoadFloat4(&localRotation), // 回転値 (クォータニオン)
		XMLoadFloat3(&localPosition) // 平行移動値
	);
}

// ゲームオブジェクトをレンダリングする関数
void GameObject::Render(ID3D11DeviceContext4* deviceContext) const noexcept
{
	// 必要なリソースが揃っていない場合は描画をスキップ
	if (!mesh || !material || !inputLayout) return;

	// 頂点バッファの設定
	const auto vertexBuffer = mesh->GetVertexBuffer();
	ID3D11Buffer* const vertexBuffers[] = { vertexBuffer->GetNativePointer(), };
	const UINT strides[] = { vertexBuffer->GetStride(), }; // 頂点のストライド（サイズ）
	const UINT offsets[] = { mesh->GetVertexOffset(), }; // 頂点バッファのオフセット
	deviceContext->IASetVertexBuffers(0, static_cast<UINT>(std::size(vertexBuffers)), vertexBuffers, strides, offsets);

	// 入力レイアウトの設定
	deviceContext->IASetInputLayout(inputLayout->GetNativePointer());

	// インデックスバッファの設定
	const auto indexBuffer = mesh->GetIndexBuffer();
	deviceContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), indexBuffer->GetFormat(), mesh->GetIndexOffset());

	// プリミティブトポロジーの設定
	deviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

	// マテリアルの適用
	material->Apply(deviceContext);

	// インデックス付き描画コール
	deviceContext->DrawIndexed(
		indexBuffer->GetCount(), // 描画するインデックス数
		mesh->GetStartIndexLocation(), // 開始インデックス位置
		mesh->GetBaseVertexLocation() // 基準頂点位置
	);
}