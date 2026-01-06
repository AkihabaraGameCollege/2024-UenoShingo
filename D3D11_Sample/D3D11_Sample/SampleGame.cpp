#include "SampleGame.h"

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
		0xFFB56300, 0xFF00F1FF, 0xFFB56300, 0xFF00F1FF,
		0xFF00F1FF, 0xFFB56300, 0xFF00F1FF, 0xFFB56300,
		0xFFB56300, 0xFF00F1FF, 0xFFB56300, 0xFF00F1FF,
		0xFF00F1FF, 0xFFB56300, 0xFF00F1FF, 0xFFB56300,
	};

	// スプライト専用の赤/黄色テクスチャ
	constexpr uint32_t spriteTextureSource[] = {
		0xFFFF0000, 0xFFFFFF00, 0xFFFF0000, 0xFFFFFF00,
		0xFFFFFF00, 0xFFFF0000, 0xFFFFFF00, 0xFFFF0000,
		0xFFFF0000, 0xFFFFFF00, 0xFFFF0000, 0xFFFFFF00,
		0xFFFFFF00, 0xFFFF0000, 0xFFFFFF00, 0xFFFF0000,
	};

	// スプライト用リソース（このファイル内で保持）
	std::unique_ptr<Mesh> spriteMesh;
	std::unique_ptr<Material> spriteMaterial;
	std::shared_ptr<InputLayout> spriteInputLayout;
	XMFLOAT4 spriteColor = { 1, 1, 1, 1 };
	XMFLOAT3 spritePosition = { -1.5f, 0.0f, 0.0f };
	XMFLOAT4 spriteRotation = { 0, 0, 0, 1 };
	XMVECTOR spriteScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
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

	// マテリアル（3D）
	albedoColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// メッシュ（3Dオブジェクト：キューブ）
	mesh = std::make_unique<Mesh>(device.Get());
	mesh->SetVertexBuffer(Vertex_Base::GetSize(), static_cast<UINT>(std::size(cubeVertices)), cubeVertices);
	mesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh->SetIndexBuffer(IndexFormat::UInt32, static_cast<UINT>(std::size(cubeIndices)), cubeIndices);

	// シェーダー（3D）
	auto shader = shaderManager->Find(L"Shader/Standard");
	// マテリアル（3D）
	material = std::make_unique<Material>(device.Get(), constantBufferManager, shader);
	material->SetColor("AlbedoColor", albedoColor);
	// テクスチャ（3D）
	auto mainTexture = std::make_shared<Texture2D>(device.Get(), 4, 4, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, textureSource);
	material->SetTexture("MainTexture", mainTexture);

	// 入力レイアウト（3D）
	inputLayout = std::make_shared<InputLayout_Base>(device.Get());

	// ===== スプライトの準備 =====
	// メッシュ（スプライト：矩形）
	spriteMesh = std::make_unique<Mesh>(device.Get());
	spriteMesh->SetVertexBuffer(Vertex_Sprite::GetSize(), static_cast<UINT>(std::size(quadVertices)), quadVertices);
	spriteMesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	spriteMesh->SetIndexBuffer(IndexFormat::UInt32, static_cast<UINT>(std::size(quadIndices)), quadIndices);

	// シェーダー（スプライト）
	auto spriteShader = shaderManager->Find(L"Shader/Sprite");
	// マテリアル（スプライト）
	spriteMaterial = std::make_unique<Material>(device.Get(), constantBufferManager, spriteShader);
	spriteMaterial->SetColor("AlbedoColor", spriteColor);
	// テクスチャ（スプライト：赤/黄色）
	// フォーマットを BGRA に変更して 0xAARRGGBB を正しく表示
	auto spriteTexture = std::make_shared<Texture2D>(
		device.Get(),
		4, 4,
		DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, // ← ここを変更
		spriteTextureSource);
	spriteMaterial->SetTexture("MainTexture", spriteTexture);

	// 入力レイアウト（スプライト）
	spriteInputLayout = std::make_shared<InputLayout_Sprite>(device.Get());
}

/// <summary>
/// フレームの更新処理を実装します。
/// </summary>
void SampleGame::OnUpdate()
{
	// 既存の回転
	const auto rotationVelocity = XMQuaternionRotationRollPitchYaw(0, XMConvertToRadians(90), 0);
	localRotation = XMQuaternionSlerp(localRotation, XMQuaternionMultiply(localRotation, rotationVelocity), Time::GetDeltaTime());

	// ===== 入力による移動 =====
	const float dt = Time::GetDeltaTime();
	const float moveSpeed3D = 2.0f;     // キューブの移動速度（units/秒）
	const float moveSpeed2D = 2.5f;     // スプライトの移動速度（units/秒）

	// ---- キューブ（WASD + Space/Ctrl + ゲームパッド左スティック） ----
	// ローカル移動入力（カメラ基準で変換するための raw 入力）
	float rawX = 0.0f; // 右方向が正
	float rawY = 0.0f; // 上方向が正
	float rawZ = 0.0f; // 前方向が正

	if (Input::GetButton(DigitalInput::A)) rawX -= 1.0f;
	if (Input::GetButton(DigitalInput::D)) rawX += 1.0f;
	if (Input::GetButton(DigitalInput::W)) rawZ += 1.0f;
	if (Input::GetButton(DigitalInput::S)) rawZ -= 1.0f;
	if (Input::GetButton(DigitalInput::Space)) rawY += 1.0f;
	if (Input::GetButton(DigitalInput::LeftCtrl)) rawY -= 1.0f;

	// ゲームパッド左スティック（X: 左右, Y: 前後）
	rawX += Input::GetAxis(AnalogInput::LeftStickX);
	// スティックは上が負値のことが多いので、左スティックY をそのまま加えると
	// 上が負になる設計の場合は符号反転しない現状コードに合わせる
	rawZ += Input::GetAxis(AnalogInput::LeftStickY);

	if (rawX != 0.0f || rawY != 0.0f || rawZ != 0.0f) {
		// カメラの向きから前方向と右方向を計算（Y成分はゼロにして水平移動にする）
		const auto cameraWorldMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&cameraRotation));
		XMVECTOR camForward = cameraWorldMatrix.r[2]; // カメラの前方向（右手系では -Z になることがあるが、このプロジェクトでは r[2] を使用）
		XMVECTOR camRight = cameraWorldMatrix.r[0];

		// Y 成分を除去して水平成分だけで移動させる（必要に応じて Y を許可）
		camForward = XMVectorSet(XMVectorGetX(camForward), 0.0f, XMVectorGetZ(camForward), 0.0f);
		camRight   = XMVectorSet(XMVectorGetX(camRight),   0.0f, XMVectorGetZ(camRight),   0.0f);

		// 正規化（ゼロベクトル対策）
		if (XMVector3Equal(XMVector3LengthSq(camForward), XMVectorZero())) {
			camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		}
		if (XMVector3Equal(XMVector3LengthSq(camRight), XMVectorZero())) {
			camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		}
		camForward = XMVector3Normalize(camForward);
		camRight = XMVector3Normalize(camRight);

		// 合成（右 * rawX + 前 * rawZ + 上 * rawY）
		const XMVECTOR worldMoveDir = XMVectorAdd(
			XMVectorScale(camRight, rawX),
			XMVectorAdd(XMVectorScale(camForward, rawZ), XMVectorSet(0.0f, rawY, 0.0f, 0.0f))
		);

		// 正規化して速度一定に
		const XMVECTOR normDir = XMVector3Normalize(worldMoveDir);
		const XMVECTOR delta = XMVectorScale(normDir, moveSpeed3D * dt);

		// localPosition は XMVECTOR と想定されるため加算
		localPosition = XMVectorAdd(localPosition, delta);
	}

	// ---- スプライト（矢印キー + マウスホイールで Z） ----
	float spriteMoveX = 0.0f, spriteMoveY = 0.0f, spriteMoveZ = 0.0f;
	if (Input::GetButton(DigitalInput::LeftArrow))  spriteMoveX -= 1.0f;
	if (Input::GetButton(DigitalInput::RightArrow)) spriteMoveX += 1.0f;
	if (Input::GetButton(DigitalInput::UpArrow))    spriteMoveY += 1.0f;
	if (Input::GetButton(DigitalInput::DownArrow))  spriteMoveY -= 1.0f;

	// マウスホイールで前後
	spriteMoveZ += Input::GetAxis(AnalogInput::MouseScroll) * 0.25f;

	if (spriteMoveX != 0.0f || spriteMoveY != 0.0f || spriteMoveZ != 0.0f) {
		// 斜め移動の速度調整
		const XMVECTOR dir = XMVector3Normalize(XMVectorSet(spriteMoveX, spriteMoveY, spriteMoveZ, 0.0f));
		const XMVECTOR delta = XMVectorScale(dir, moveSpeed2D * dt);
		XMFLOAT3 d;
		XMStoreFloat3(&d, delta);
		spritePosition.x += d.x;
		spritePosition.y += d.y;
		spritePosition.z += d.z;
	}
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

	// ===== 3Dオブジェクトの描画（キューブ） =====
	{
		const auto matrixWorld = XMMatrixTransformation(
			XMVectorZero(), XMQuaternionIdentity(), localScale,
			XMVectorZero(), localRotation,
			localPosition);
		XMStoreFloat4x4(&constantsPerDraw.MatrixWorld, XMMatrixTranspose(matrixWorld));
		constantBufferPerDraw->UpdateSubresource(&constantsPerDraw);

		const auto vertexBuffer = mesh->GetVertexBuffer();
		ID3D11Buffer* const vertexBuffers[] = { vertexBuffer->GetNativePointer(), };
		const UINT strides[] = { vertexBuffer->GetStride(), };
		const UINT offsets[] = { mesh->GetVertexOffset(), };
		deviceContext->IASetVertexBuffers(0, static_cast<UINT>(std::size(vertexBuffers)), vertexBuffers, strides, offsets);
		deviceContext->IASetInputLayout(inputLayout->GetNativePointer());
		const auto indexBuffer = mesh->GetIndexBuffer();
		deviceContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), indexBuffer->GetFormat(), mesh->GetIndexOffset());
		deviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());
		material->Apply(deviceContext.Get());
		deviceContext->DrawIndexed(indexBuffer->GetCount(), mesh->GetStartIndexLocation(), mesh->GetBaseVertexLocation());
	}

	// ===== スプライトの描画（矩形） =====
	{
		// スプライト用のワールド行列（位置だけ変更）
		const auto spriteWorld = XMMatrixTransformation(
			XMVectorZero(), XMQuaternionIdentity(), spriteScale,
			XMVectorZero(), XMLoadFloat4(&spriteRotation),
			XMLoadFloat3(&spritePosition));
		XMStoreFloat4x4(&constantsPerDraw.MatrixWorld, XMMatrixTranspose(spriteWorld));
		constantBufferPerDraw->UpdateSubresource(&constantsPerDraw);

		const auto vertexBuffer = spriteMesh->GetVertexBuffer();
		ID3D11Buffer* const vertexBuffers[] = { vertexBuffer->GetNativePointer(), };
		const UINT strides[] = { vertexBuffer->GetStride(), };
		const UINT offsets[] = { spriteMesh->GetVertexOffset(), };
		deviceContext->IASetVertexBuffers(0, static_cast<UINT>(std::size(vertexBuffers)), vertexBuffers, strides, offsets);
		deviceContext->IASetInputLayout(spriteInputLayout->GetNativePointer());
		const auto indexBuffer = spriteMesh->GetIndexBuffer();
		deviceContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), indexBuffer->GetFormat(), spriteMesh->GetIndexOffset());
		deviceContext->IASetPrimitiveTopology(spriteMesh->GetPrimitiveTopology());
		spriteMaterial->Apply(deviceContext.Get());
		deviceContext->DrawIndexed(indexBuffer->GetCount(), spriteMesh->GetStartIndexLocation(), spriteMesh->GetBaseVertexLocation());
	}
}