#include "SampleGame.h"
#include "StandardVertexShader.h"

using namespace GameLibrary;
using namespace DirectX;

namespace
{
	// 頂点データの配列
	constexpr Vertex_Sprite vertices[] = {
		{ { -1.0f, +1.0f, +0.0f, }, },
		{ { +1.0f, +1.0f, +0.0f, }, },
		{ { -1.0f, -1.0f, +0.0f, }, },
	};

	// キューブ（頂点）
	constexpr VertexPositionNormal vertices_cube[] = {
		// Front
		{ {  0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
		{ { -0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
		// Back
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f } },
		// Right
		{ { 0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f } },
		// Left
		{ { -0.5f,  0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f } },
		// Top
		{ { -0.5f, 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f } },
		{ {  0.5f, 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f } },
		{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
		{ {  0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
		// Bottom
		{ {  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f } },
	};
	// キューブ（インデックス）
	constexpr UINT32 indices_cube[] = {
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
	HRESULT hr = S_OK;

	// 頂点バッファー
	vertexBuffer = std::make_shared<VertexBuffer>(
		device.Get(),
		Vertex_Sprite::GetSize(),
		static_cast<UINT>(std::size(vertices)),
		vertices);
	vertexOffset = 0;

	// シェーダー
	vertexShader = spriteVertexShader.get();
	geometryShader = spriteGeometryShader.get();
	pixelShader = spritePixelShader.get();

	{
		// 作成するインデックス バッファーについての記述
		constexpr auto bufferDesc = D3D11_BUFFER_DESC{
			.ByteWidth = sizeof vertices,
			.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = 0,
		};
		// バッファーを作成
		hr = device->CreateBuffer(&bufferDesc, NULL, &indexBuffer);
		ThrowIfFailed(hr);
	}
	// バッファーにデータを転送
	deviceContext->UpdateSubresource(indexBuffer.Get(), 0, NULL, indices_cube, 0, 0);
	indexCount = std::size(indices_cube);

	// 入力レイアウトを作成
	hr = device->CreateInputLayout(
		VertexPositionNormal::InputElementDescs, std::size(VertexPositionNormal::InputElementDescs),
		g_StandardVertexShader, std::size(g_StandardVertexShader),
		&inputLayout);
	ThrowIfFailed(hr);

	// 定数バッファーを作成
	{
		// 作成するバッファーについての記述
		constexpr auto bufferDesc = D3D11_BUFFER_DESC{
			.ByteWidth = sizeof(ConstantBufferPerFrame),
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = 0,
		};
		// バッファーを作成
		auto hr = device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
		ThrowIfFailed(hr);
	}
	// 定数バッファーを更新
	XMStoreFloat4x4(&constantBufferPerFrame.worldMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&constantBufferPerFrame.viewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&constantBufferPerFrame.projectionMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&constantBufferPerFrame.wvpMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	constantBufferPerFrame.lightPosition = XMFLOAT4(1, 2, -2, 1);
	constantBufferPerFrame.materialDiffuseColor = XMFLOAT4(1, 238 / 255.0f, 0, 1);
	deviceContext->UpdateSubresource(constantBuffer.Get(), 0, NULL, &constantBufferPerFrame, 0, 0);
}

/// <summary>
/// フレームの更新処理を実装します。
/// </summary>
void SampleGame::OnUpdate()
{
	time += Time::GetDeltaTime();

	// 位置座標
	XMFLOAT3 position = { 0, 0, 0 };
	// 回転
	XMFLOAT4 rotation = {};
	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	// スケール
	XMFLOAT3 scale = { 1, 1, 1 };

	// y軸回転
	const float xAngle = XMConvertToRadians(45 * time);
	const float yAngle = XMConvertToRadians(90 * time);
	XMStoreFloat4(&rotation,
		XMQuaternionRotationRollPitchYaw(xAngle, yAngle, 0));

	// 定数バッファーを更新
	const auto worldMatrix = XMMatrixTransformation(
		XMVectorZero(), XMQuaternionIdentity(), XMLoadFloat3(&scale),
		XMVectorZero(), XMLoadFloat4(&rotation),
		XMLoadFloat3(&position));
	XMStoreFloat4x4(&constantBufferPerFrame.worldMatrix, XMMatrixTranspose(worldMatrix));

	// マテリアル
	if (GetAsyncKeyState('D')) {
		constantBufferPerFrame.materialDiffuseColor = DirectX::XMFLOAT4(1, 1, 0, 1);
	}
	else {
		constantBufferPerFrame.materialDiffuseColor = DirectX::XMFLOAT4(0, 0, 0, 1);
	}
	if (GetAsyncKeyState('S')) {
		constantBufferPerFrame.materialSpecularColor = DirectX::XMFLOAT3(1, 1, 1);
		constantBufferPerFrame.materialSpecularPower = 1;
	}
	else {
		constantBufferPerFrame.materialSpecularColor = DirectX::XMFLOAT3(0, 0, 0);
	}

	// ライト
	constantBufferPerFrame.lightPosition = DirectX::XMFLOAT4(1.0f, 2.0f, -2.0f, 0.0f);

	// カメラの位置座標
	constexpr XMFLOAT3 eyePosition = { 0.0f, 0.0f, -10.0f };
	// カメラの回転
	XMFLOAT4 cameraRotation = {};
	XMStoreFloat4(&cameraRotation, XMQuaternionIdentity());

	// 定数バッファーを更新
	const auto eyeDirection = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&cameraRotation));
	const auto eyeUpDirection = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), XMLoadFloat4(&cameraRotation));
	const auto viewMatrix = XMMatrixLookToLH(
		XMLoadFloat3(&eyePosition), eyeDirection, eyeUpDirection);
	XMStoreFloat4x4(&constantBufferPerFrame.viewMatrix, XMMatrixTranspose(viewMatrix));


	// スクリーン画面のアスペクト比
	const auto aspectRatio = GetWidth() / static_cast<float>(GetHeight());
	constexpr auto nearZ = 0.3f;	// nearクリップ面
	constexpr auto farZ = 1000.0f;	// farクリップ面

	//// 【正射影変換の場合】
	//constexpr auto orthographicSize = 5.0f;	// ビュー空間の垂直方向の半分のサイズ
	//// 定数バッファーを更新
	//const auto projectionMatrix = XMMatrixOrthographicLH(
	//	2 * orthographicSize * aspectRatio,
	//	2 * orthographicSize, nearZ, farZ);

	// 【パースペクティブ射影変換の場合】
	// 視錐台の垂直方向の角度
	constexpr auto fieldOfView = XMConvertToRadians(60);
	const auto projectionMatrix = XMMatrixPerspectiveFovLH(
		fieldOfView, aspectRatio, nearZ, farZ);

	// 定数バッファーを更新
	XMStoreFloat4x4(&constantBufferPerFrame.projectionMatrix, XMMatrixTranspose(projectionMatrix));

	// ワールド × ビュー × プロジェクションをCPU側で計算してシェーダーへ送る
	XMStoreFloat4x4(&constantBufferPerFrame.wvpMatrix,
		XMMatrixTranspose(worldMatrix * viewMatrix * projectionMatrix));
}

/// <summary>
/// フレームの描画処理を実装します。
/// </summary>
void SampleGame::OnRender()
{
	// 頂点バッファーを設定
	ID3D11Buffer* const vertexBuffers[] = { vertexBuffer->GetNativePointer(), };
	const UINT strides[] = { vertexBuffer->GetStride(), };
	const UINT offsets[] = { vertexOffset, };
	deviceContext->IASetVertexBuffers(0, static_cast<UINT>(std::size(vertexBuffers)), vertexBuffers, strides, offsets);

	// Shaders
	vertexShader->Apply(deviceContext.Get());
	geometryShader->Apply(deviceContext.Get());
	pixelShader->Apply(deviceContext.Get());

	// インデックス バッファーを設定
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	// 頂点バッファーと頂点シェーダーの組合せに対応した入力レイアウトを設定
	deviceContext->IASetInputLayout(inputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファーを設定
	deviceContext->UpdateSubresource(constantBuffer.Get(), 0, NULL, &constantBufferPerFrame, 0, 0);
	ID3D11Buffer* constantBuffers[] = { constantBuffer.Get(), };
	deviceContext->VSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);
	deviceContext->GSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);
	deviceContext->PSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);

	// メッシュを描画
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
