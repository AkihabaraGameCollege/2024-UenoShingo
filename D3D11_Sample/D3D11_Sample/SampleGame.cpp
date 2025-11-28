#include "SampleGame.h"

using namespace GameLibrary;
using namespace DirectX;

namespace
{
	// 矩形（頂点）
	constexpr Vertex_Sprite quadVertices[] = {
		{ { -0.5f, +0.5f, +0.0f, }, },
		{ { +0.5f, +0.5f, +0.0f, }, },
		{ { -0.5f, -0.5f, +0.0f, }, },
		{ { +0.5f, -0.5f, +0.0f, }, },
	};
	// 矩形（インデックス）
	constexpr uint32_t quadIndices[] = {
		0, 1, 2,
		3, 2, 1,
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
	// マテリアル
	albedoColor = { 0x80 / 255.0f, 0xBB / 255.0f, 0xFF / 255.0f, 1.0f };

	// 頂点バッファー
	vertexBuffer = std::make_shared<VertexBuffer>(
		device.Get(),
		Vertex_Sprite::GetSize(),
		static_cast<UINT>(std::size(quadVertices)),
		quadVertices);
	vertexOffset = 0;

	// インデックス バッファー
	indexBuffer = std::make_shared<IndexBuffer>(
		device.Get(),
		IndexFormat::UInt32, static_cast<UINT>(std::size(quadIndices)),
		quadIndices);
	indexOffset = 0;

	// シェーダー
	vertexShader = spriteVertexShader.get();
	geometryShader = spriteGeometryShader.get();
	pixelShader = spritePixelShader.get();
	// マテリアル
	constantBufferPerMaterial = std::make_shared<ConstantBuffer>(device.Get(),
		static_cast<UINT>(sizeof constantsPerMaterial));

	// 入力レイアウト
	inputLayout = std::make_shared<InputLayout_Sprite>(device.Get());

	startIndexLocation = 0;
	baseVertexLocation = 0;
}

/// <summary>
/// フレームの更新処理を実装します。
/// </summary>
void SampleGame::OnUpdate()
{
	time += Time::GetDeltaTime();
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
	deviceContext->IASetInputLayout(inputLayout->GetNativePointer());
	// マテリアル
	XMStoreFloat4(&constantsPerMaterial.Albedo, XMColorSRGBToRGB(XMLoadFloat4(&albedoColor)));
	constantBufferPerMaterial->UpdateSubresource(&constantsPerMaterial);
	// Constant buffer
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerMaterial->GetNativePointer(),
		};
		//deviceContext->VSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);
	}
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerMaterial->GetNativePointer(),
		};
		//deviceContext->GSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);
	}
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerMaterial->GetNativePointer(),
		};
		deviceContext->PSSetConstantBuffers(0, std::size(constantBuffers), constantBuffers);
	}
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
