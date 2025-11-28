#pragma once

#include <GameLibrary/Graphics.h>
#include <wrl/client.h>
#include <d3d11_4.h>

namespace GameLibrary
{
	/// <summary>
	/// 頂点シェーダーを表します。
	/// </summary>
	class VertexShader final : public GraphicsResource
	{
	public:
		VertexShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~VertexShader() = default;

		ID3D11VertexShader* GetNativePointer() const noexcept;

		void Apply(ID3D11DeviceContext4* deviceContext) noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	};

	/// <summary>
	/// ピクセル シェーダーを表します。
	/// </summary>
	class PixelShader final : public GraphicsResource
	{
	public:
		PixelShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~PixelShader() = default;

		ID3D11PixelShader* GetNativePointer() const noexcept;

		void Apply(ID3D11DeviceContext4* deviceContext) noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	};
}
