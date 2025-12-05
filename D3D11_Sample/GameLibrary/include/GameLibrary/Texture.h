#pragma once

#include <GameLibrary/Graphics.h>
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11_4.h>

namespace GameLibrary
{
	class Texture2D final : public GraphicsResource
	{
	public:
		Texture2D(ID3D11Device5* graphicsDevice, UINT width, UINT height, DXGI_FORMAT format, const void* source);

		UINT GetWidth() const noexcept;
		UINT GetHeight() const noexcept;

		ID3D11Texture2D1* GetNativePointer() noexcept;
		ID3D11ShaderResourceView1* GetView() noexcept;
		ID3D11SamplerState* GetSamplerState() noexcept;

	private:
		UINT width;
		UINT height;
		DXGI_FORMAT format;
		Microsoft::WRL::ComPtr<ID3D11Texture2D1> nativePointer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> view;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	};
}
