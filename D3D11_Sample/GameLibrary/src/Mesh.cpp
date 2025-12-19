#include <GameLibrary/Mesh.h>

using namespace GameLibrary;

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphicsDevice">グラフィックス デバイス</param>
Mesh::Mesh(ID3D11Device5* graphicsDevice)
	: GraphicsResource(graphicsDevice)
{

}

VertexBuffer* Mesh::GetVertexBuffer() noexcept
{
	return vertexBuffer.get();
}

const VertexBuffer* Mesh::GetVertexBuffer() const noexcept
{
	return vertexBuffer.get();
}

UINT Mesh::GetVertexOffset() const noexcept
{
	return vertexOffset;
}

IndexBuffer* Mesh::GetIndexBuffer() noexcept
{
	return indexBuffer.get();
}

D3D11_PRIMITIVE_TOPOLOGY Mesh::GetPrimitiveTopology() const noexcept
{
	return primitiveTopology;
}

const IndexBuffer* Mesh::GetIndexBuffer() const noexcept
{
	return indexBuffer.get();
}

UINT Mesh::GetIndexOffset() const noexcept
{
	return indexOffset;
}

UINT Mesh::GetStartIndexLocation() const noexcept
{
	return startIndexLocation;
}

INT Mesh::GetBaseVertexLocation() const noexcept
{
	return baseVertexLocation;
}

void Mesh::SetVertexBuffer(UINT vertexStride, UINT vertexCount, const void* source)
{
	vertexBuffer = std::make_unique<VertexBuffer>(GetDevice(), vertexStride, vertexCount, source);
}

void Mesh::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY value)
{
	primitiveTopology = value;
}

void Mesh::SetIndexBuffer(IndexFormat format, UINT indexCount, const void* source)
{
	indexBuffer = std::make_unique<IndexBuffer>(GetDevice(), format, indexCount, source);
}

void Mesh::SetStartIndexLocation(UINT value) noexcept
{
	startIndexLocation = value;
}

void Mesh::SetBaseVertexLocation(INT value) noexcept
{
	baseVertexLocation = value;
}
