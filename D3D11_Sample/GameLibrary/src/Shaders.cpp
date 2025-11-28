#include <GameLibrary/Shader.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

VertexShader::VertexShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	ThrowIfFailed(graphicsDevice->CreateVertexShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

ID3D11VertexShader* VertexShader::GetNativePointer() const noexcept
{
	return shader.Get();
}

void VertexShader::Apply(ID3D11DeviceContext4* deviceContext) noexcept
{
	deviceContext->VSSetShader(shader.Get(), NULL, 0);
}

GeometryShader::GeometryShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	ThrowIfFailed(graphicsDevice->CreateGeometryShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

ID3D11GeometryShader* GeometryShader::GetNativePointer() const noexcept
{
	return shader.Get();
}

void GeometryShader::Apply(ID3D11DeviceContext4* deviceContext) noexcept
{
	deviceContext->GSSetShader(shader.Get(), NULL, 0);
}

PixelShader::PixelShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	ThrowIfFailed(graphicsDevice->CreatePixelShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

ID3D11PixelShader* PixelShader::GetNativePointer() const noexcept
{
	return shader.Get();
}

void PixelShader::Apply(ID3D11DeviceContext4* deviceContext) noexcept
{
	deviceContext->PSSetShader(shader.Get(), NULL, 0);
}
