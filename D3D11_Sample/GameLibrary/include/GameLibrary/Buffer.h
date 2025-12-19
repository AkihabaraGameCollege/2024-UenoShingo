#pragma once

#include <GameLibrary/Graphics.h>
#include <Windows.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <wrl/client.h>
#include <d3d11_4.h>

namespace GameLibrary
{
	class VertexBuffer final : public GraphicsResource
	{
	public:
		VertexBuffer(ID3D11Device5* graphicsDevice, UINT vertexStride, UINT vertexCount, const void* source);

		UINT GetStride() const noexcept;
		UINT GetCount() const noexcept;
		ID3D11Buffer* GetNativePointer() noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT stride = 0;
		UINT count = 0;
	};

	enum class IndexFormat
	{
		UInt16,
		UInt32,
	};

	class IndexBuffer final : public GraphicsResource
	{
	public:
		IndexBuffer(ID3D11Device5* graphicsDevice, IndexFormat format, UINT indexCount, const void* source);

		DXGI_FORMAT GetFormat() const noexcept;
		UINT GetCount() const noexcept;
		ID3D11Buffer* GetNativePointer() noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		UINT count = 0;
	};

	class ConstantBuffer final : public GraphicsResource
	{
	public:
		ConstantBuffer(ID3D11Device5* graphicsDevice, UINT byteWidth);

		UINT GetByteWidth() const noexcept;
		void UpdateSubresource(const void* source) noexcept;

		ID3D11Buffer* GetNativePointer() noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT byteWidth = 0;
	};

	class ConstantBufferManager final : public GraphicsResource
	{
		typedef std::unordered_map<size_t, std::shared_ptr<ConstantBuffer>> ConstantBuffersType;

	public:
		explicit ConstantBufferManager(ID3D11Device5* graphicsDevice);
		~ConstantBufferManager() = default;

		void Add(const std::string& name, std::shared_ptr<ConstantBuffer> constantBuffer);
		ConstantBuffer* Find(size_t nameId);
		ConstantBuffer* Find(const std::string& name);

	private:
		ConstantBuffersType constantBuffers;
	};
}
