#include <GameLibrary/Shader.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

VertexShader::VertexShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: graphicsDevice(graphicsDevice)
{
	ThrowIfFailed(graphicsDevice->CreateVertexShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

/// <summary>
/// このリソースを作成したグラフィックス デバイスを取得します。
/// </summary>
/// <returns>グラフィックス デバイス</returns>
ID3D11Device5* VertexShader::GetDevice() noexcept
{
	return graphicsDevice.Get();
}

ID3D11VertexShader* VertexShader::GetNativePointer() const noexcept
{
	return shader.Get();
}

void VertexShader::Apply(ID3D11DeviceContext4* deviceContext) noexcept
{
	deviceContext->VSSetShader(shader.Get(), NULL, 0);
}
