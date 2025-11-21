#pragma once

#include <GameLibrary/Graphics.h>
#include <Windows.h>
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
}
