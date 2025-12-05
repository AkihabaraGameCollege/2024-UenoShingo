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
		{ { +0.5f, +0.5f, +0.5f, }, { +0.0f, +0.0f, +1.0f }, },
		{ { -0.5f, +0.5f, +0.5f, }, { +0.0f, +0.0f, +1.0f }, },
		{ { +0.5f, -0.5f, +0.5f, }, { +0.0f, +0.0f, +1.0f }, },
		{ { -0.5f, -0.5f, +0.5f, }, { +0.0f, +0.0f, +1.0f }, },
		// Back
		{ { -0.5f, +0.5f, -0.5f, }, { +0.0f, +0.0f, -1.0f }, },
		{ { +0.5f, +0.5f, -0.5f, }, { +0.0f, +0.0f, -1.0f }, },
		{ { -0.5f, -0.5f, -0.5f, }, { +0.0f, +0.0f, -1.0f }, },
		{ { +0.5f, -0.5f, -0.5f, }, { +0.0f, +0.0f, -1.0f }, },
		// Right
		{ { +0.5f, +0.5f, -0.5f, }, { +1.0f, +0.0f, +0.0f }, },
		{ { +0.5f, +0.5f, +0.5f, }, { +1.0f, +0.0f, +0.0f }, },
		{ { +0.5f, -0.5f, -0.5f, }, { +1.0f, +0.0f, +0.0f }, },
		{ { +0.5f, -0.5f, +0.5f, }, { +1.0f, +0.0f, +0.0f }, },
		// Left
		{ { -0.5f, +0.5f, +0.5f, }, { -1.0f, +0.0f, +0.0f }, },
		{ { -0.5f, +0.5f, -0.5f, }, { -1.0f, +0.0f, +0.0f }, },
		{ { -0.5f, -0.5f, +0.5f, }, { -1.0f, +0.0f, +0.0f }, },
		{ { -0.5f, -0.5f, -0.5f, }, { -1.0f, +0.0f, +0.0f }, },
		// Top
		{ { -0.5f, +0.5f, +0.5f, }, { +0.0f, +1.0f, +0.0f }, },
		{ { +0.5f, +0.5f, +0.5f, }, { +0.0f, +1.0f, +0.0f }, },
		{ { -0.5f, +0.5f, -0.5f, }, { +0.0f, +1.0f, +0.0f }, },
		{ { +0.5f, +0.5f, -0.5f, }, { +0.0f, +1.0f, +0.0f }, },
		// Bottom
		{ { +0.5f, -0.5f, +0.5f, }, { +0.0f, -1.0f, +0.0f }, },
		{ { -0.5f, -0.5f, +0.5f, }, { +0.0f, -1.0f, +0.0f }, },
		{ { +0.5f, -0.5f, -0.5f, }, { +0.0f, -1.0f, +0.0f }, },
		{ { -0.5f, -0.5f, -0.5f, }, { +0.0f, -1.0f, +0.0f }, },
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
	// 平行ライト
	XMStoreFloat4(&lightRotation, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(50),
		XMConvertToRadians(-30),
		XMConvertToRadians(0)));
	lightColor = { 1, 1, 1, 1 };

	constantBufferPerLighting = std::make_shared<ConstantBuffer>(device.Get(),
		static_cast<UINT>(sizeof constantsPerLighting));

	constantBufferPerFrame = std::make_shared<ConstantBuffer>(device.Get(),
		static_cast<UINT>(sizeof constantsPerFrame));

	constantBufferPerDraw = std::make_shared<ConstantBuffer>(device.Get(),
		static_cast<UINT>(sizeof constantsPerDraw));

	// マテリアル
	albedoColor = { 0x80 / 255.0f, 0xBB / 255.0f, 0xFF / 255.0f, 1.0f };

	// 頂点バッファー
	vertexBuffer = std::make_shared<VertexBuffer>(
		device.Get(),
		Vertex_Base::GetSize(),
		static_cast<UINT>(std::size(cubeVertices)),
		cubeVertices);
	vertexOffset = 0;

	// インデックス バッファー
	indexBuffer = std::make_shared<IndexBuffer>(
		device.Get(),
		IndexFormat::UInt32, static_cast<UINT>(std::size(cubeIndices)),
		cubeIndices);
	indexOffset = 0;

	// シェーダー
	vertexShader = standardVertexShader.get();
	geometryShader = standardGeometryShader.get();
	pixelShader = standardPixelShader.get();
	// テクスチャ
	mainTexture = std::make_shared<Texture2D>(device.Get(), 4, 4, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, textureSource);
	// マテリアル
	constantBufferPerMaterial = std::make_shared<ConstantBuffer>(device.Get(),
		static_cast<UINT>(sizeof constantsPerMaterial));

	// 入力レイアウト
	inputLayout = std::make_shared<InputLayout_Base>(device.Get());

	startIndexLocation = 0;
	baseVertexLocation = 0;
}

/// <summary>
/// フレームの更新処理を実装します。
/// </summary>
void SampleGame::OnUpdate()
{
	const auto rotationVelocity = XMQuaternionRotationRollPitchYaw(0, XMConvertToRadians(90), 0);
	localRotation = XMQuaternionSlerp(localRotation, XMQuaternionMultiply(localRotation, rotationVelocity), Time::GetDeltaTime());
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

	// ゲーム オブジェクト
	const auto matrixWorld = XMMatrixTransformation(
		XMVectorZero(), XMQuaternionIdentity(), localScale,
		XMVectorZero(), localRotation,
		localPosition);
	XMStoreFloat4x4(&constantsPerDraw.MatrixWorld, XMMatrixTranspose(matrixWorld));
	constantBufferPerDraw->UpdateSubresource(&constantsPerDraw);

	// 頂点バッファーを設定
	ID3D11Buffer* const vertexBuffers[] = { vertexBuffer->GetNativePointer(), };
	const UINT strides[] = { vertexBuffer->GetStride(), };
	const UINT offsets[] = { vertexOffset, };
	deviceContext->IASetVertexBuffers(0, static_cast<UINT>(std::size(vertexBuffers)), vertexBuffers, strides, offsets);
	deviceContext->IASetInputLayout(inputLayout->GetNativePointer());
	// マテリアル
	XMStoreFloat4(&constantsPerMaterial.Albedo, XMColorSRGBToRGB(XMLoadFloat4(&albedoColor)));
	constantBufferPerMaterial->UpdateSubresource(&constantsPerMaterial);
	// Constant buffer
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerDraw->GetNativePointer(),
			constantBufferPerFrame->GetNativePointer(),
		};
		//deviceContext->VSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);
	}
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerDraw->GetNativePointer(),
			constantBufferPerFrame->GetNativePointer(),
		};
		deviceContext->GSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);
	}
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerLighting->GetNativePointer(),
			constantBufferPerMaterial->GetNativePointer(),
		};
		deviceContext->PSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);
	}
	// Texture
	ID3D11ShaderResourceView* const textures[] = { mainTexture->GetView(), };
	deviceContext->VSSetShaderResources(0, std::size(textures), textures);
	deviceContext->GSSetShaderResources(0, std::size(textures), textures);
	deviceContext->PSSetShaderResources(0, std::size(textures), textures);
	ID3D11SamplerState* const samplerStates[] = { mainTexture->GetSamplerState(), };
	deviceContext->VSSetSamplers(0, std::size(samplerStates), samplerStates);
	deviceContext->GSSetSamplers(0, std::size(samplerStates), samplerStates);
	deviceContext->PSSetSamplers(0, std::size(samplerStates), samplerStates);
	// Shaders
	vertexShader->Apply(deviceContext.Get());
	geometryShader->Apply(deviceContext.Get());
	pixelShader->Apply(deviceContext.Get());

	// インデックス バッファーを設定
	deviceContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), indexBuffer->GetFormat(), indexOffset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// メッシュを描画
	deviceContext->DrawIndexed(indexBuffer->GetCount(), startIndexLocation, baseVertexLocation);
}
