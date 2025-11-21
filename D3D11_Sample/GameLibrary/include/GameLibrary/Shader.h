#pragma once

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11_4.h>

namespace GameLibrary
{
	/// <summary>
	/// 頂点シェーダーを表します。
	/// </summary>
	class VertexShader final
	{
	public:
		VertexShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~VertexShader() = default;

		ID3D11Device5* GetDevice() noexcept;
		ID3D11VertexShader* GetNativePointer() const noexcept;

		void Apply(ID3D11DeviceContext4* deviceContext) noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device5> graphicsDevice;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	};
}
