#pragma once

#include <Windows.h>
#include <iterator>
#include <wrl/client.h>
#include <d3d11_4.h>
#include <DirectXMath.h>

namespace GameLibrary
{
	struct Vertex_Base
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;

		static constexpr UINT GetSize() { return static_cast<UINT>(sizeof(Vertex_Base)); }

		static constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		static constexpr UINT NumElements = static_cast<UINT>(std::size(InputElementDescs));
	};

	struct Vertex_Sprite
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 TexCoord;

		static constexpr UINT GetSize() { return static_cast<UINT>(sizeof(Vertex_Sprite)); }

		static constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0,    DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		static constexpr UINT NumElements = static_cast<UINT>(std::size(InputElementDescs));
	};

	// 一つの頂点に含まれるデータの型
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT4 color;	// 頂点カラー

		static constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
			{ "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	// 位置座標と法線ベクトルを頂点情報に持つデータを表します。
	struct VertexPositionNormal
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル

		// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
		static constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	// 一つの頂点に含まれるデータの型
	struct VertexPositionNormalTexture
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル
		DirectX::XMFLOAT2 texCoord;	// テクスチャUV座標

		static constexpr D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	class GraphicsResource
	{
	public:
		GraphicsResource(ID3D11Device5* graphicsDevice) noexcept;
		virtual ~GraphicsResource() = default;

		ID3D11Device5* GetDevice() noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device5> graphicsDevice;
	};
}
