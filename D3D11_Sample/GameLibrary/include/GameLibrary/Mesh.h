#pragma once

#include <GameLibrary/Graphics.h>
#include <memory>
#include <Windows.h>
#include <d3d11_4.h>
#include <GameLibrary/Buffer.h>
#include <GameLibrary/InputLayout.h>
#include <GameLibrary/Material.h>

namespace GameLibrary
{
	/// <summary>
	/// メッシュを表します。
	/// </summary>
	class Mesh final : public GraphicsResource
	{
	public:
		Mesh(ID3D11Device5* graphicsDevice);
		~Mesh() = default;

		VertexBuffer* GetVertexBuffer() noexcept;
		const VertexBuffer* GetVertexBuffer() const noexcept;
		UINT GetVertexOffset() const noexcept;
		IndexBuffer* GetIndexBuffer() noexcept;
		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const noexcept;
		const IndexBuffer* GetIndexBuffer() const noexcept;
		UINT GetIndexOffset() const noexcept;
		UINT GetStartIndexLocation() const noexcept;
		INT GetBaseVertexLocation() const noexcept;

		void SetVertexBuffer(UINT vertexStride, UINT vertexCount, const void* source);
		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY value);
		void SetIndexBuffer(IndexFormat format, UINT indexCount, const void* source);
		void SetStartIndexLocation(UINT value) noexcept;
		void SetBaseVertexLocation(INT value) noexcept;

	private:
		std::unique_ptr<VertexBuffer> vertexBuffer;
		UINT vertexOffset = 0;

		std::unique_ptr<InputLayout> inputLayout;

		std::unique_ptr<IndexBuffer> indexBuffer;
		UINT indexOffset = 0;

		D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

		UINT startIndexLocation = 0;
		INT baseVertexLocation = 0;
	};
}
