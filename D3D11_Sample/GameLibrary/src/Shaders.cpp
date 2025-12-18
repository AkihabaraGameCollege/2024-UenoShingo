#include <GameLibrary/Shader.h>
#include <GameLibrary/Utility.h>
#include <format>
#include <unordered_set>
#include <d3dcompiler.h>

using namespace GameLibrary;
using Microsoft::WRL::ComPtr;

VariableDesc::VariableDesc(ID3D11ShaderReflectionVariable* variable)
{
	auto variableDesc = D3D11_SHADER_VARIABLE_DESC{};
	ThrowIfFailed(variable->GetDesc(&variableDesc));

	Name = variableDesc.Name;
	StartOffset = variableDesc.StartOffset;
	Size = variableDesc.Size;
	uFlags = variableDesc.uFlags;
	StartTexture = variableDesc.StartTexture;
	TextureSize = variableDesc.TextureSize;
	StartSampler = variableDesc.StartSampler;
	SamplerSize = variableDesc.SamplerSize;

	DefaultValue.resize(variableDesc.Size, 0);
	if (variableDesc.DefaultValue != nullptr) {
		std::memcpy(DefaultValue.data(), variableDesc.DefaultValue, variableDesc.Size);
	}

	auto bufferDesc = D3D11_SHADER_BUFFER_DESC{};
	ThrowIfFailed(variable->GetBuffer()->GetDesc(&bufferDesc));
	ConstantBufferName = bufferDesc.Name;

	const auto reflectionType = variable->GetType();
	auto typeDesc = D3D11_SHADER_TYPE_DESC{};
	ThrowIfFailed(reflectionType->GetDesc(&typeDesc));
	switch (typeDesc.Type) {
	case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
		if (typeDesc.Columns == 1 && typeDesc.Rows == 1) {
			Type = ShaderPropertyType::Float;
		}
		else if (typeDesc.Columns == 4 && typeDesc.Rows == 1) {
			Type = ShaderPropertyType::Vector;
		}
		else if (typeDesc.Columns == 4 && typeDesc.Rows == 4) {
			Type = ShaderPropertyType::Matrix;
		}
		break;
	case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT:
		if (typeDesc.Columns == 1 && typeDesc.Rows == 1) {
			Type = ShaderPropertyType::Int;
		}
		break;
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE:
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE1D:
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE2D:
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE3D:
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURECUBE:
		//	Type = ShaderPropertyType::Texture;
		//	break;
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_CBUFFER:
		//	Type = ShaderPropertyType::ConstantBuffer;
		//	break;
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_SAMPLER:
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_SAMPLER1D:
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_SAMPLER2D:
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_SAMPLER3D:
		//case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_SAMPLERCUBE:
		//	Type = ShaderPropertyType::SamplerState;
		//	break;
	default:
		break;
	}
}

ConstantBufferDesc::ConstantBufferDesc(ID3D11ShaderReflectionConstantBuffer* constantBuffer)
{
	auto bufferDesc = D3D11_SHADER_BUFFER_DESC{};
	ThrowIfFailed(constantBuffer->GetDesc(&bufferDesc));

	Name = bufferDesc.Name;
	Type = bufferDesc.Type;
	Size = bufferDesc.Size;
	uFlags = bufferDesc.uFlags;
	// グローバル変数を検索
	Variables.reserve(bufferDesc.Variables);
	for (UINT variableIndex = 0; variableIndex < bufferDesc.Variables; variableIndex++) {
		const auto reflectionVariable = constantBuffer->GetVariableByIndex(variableIndex);
		const auto& variableDesc = VariableDesc{ reflectionVariable };
		Variables.push_back(variableDesc);
	}
}

ResourceBindingDesc::ResourceBindingDesc(const D3D11_SHADER_INPUT_BIND_DESC& bindingDesc)
{
	Name = bindingDesc.Name;
	Type = bindingDesc.Type;
	BindPoint = bindingDesc.BindPoint;
	BindCount = bindingDesc.BindCount;
	uFlags = bindingDesc.uFlags;
	ReturnType = bindingDesc.ReturnType;
	Dimension = bindingDesc.Dimension;
	NumSamples = bindingDesc.NumSamples;

	if (bindingDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE) {
		SamplerName = std::format("{0}Sampler", bindingDesc.Name);
	}
	else {
		SamplerName = "";
	}
}

ReflectiveShader::ReflectiveShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	// シェーダーのリフレクションを取得
	ComPtr<ID3D11ShaderReflection> reflection;
	ThrowIfFailed(D3DReflect(shaderBytecode, bytecodeLength, IID_PPV_ARGS(&reflection)));
	auto shaderDesc = D3D11_SHADER_DESC{};
	ThrowIfFailed(reflection->GetDesc(&shaderDesc));

	// 定数バッファーを検索
	constantBufferDescs.reserve(shaderDesc.ConstantBuffers);
	for (UINT index = 0; index < shaderDesc.ConstantBuffers; index++) {
		const auto reflectionBuffer = reflection->GetConstantBufferByIndex(index);
		const auto& bufferDesc = ConstantBufferDesc{ reflectionBuffer };
		constantBufferDescs.push_back(bufferDesc);
	}

	// リソース バインディングの詳細を取得
	bindingDescs.reserve(shaderDesc.BoundResources);
	for (UINT resourceIndex = 0; resourceIndex < shaderDesc.BoundResources; resourceIndex++) {
		auto desc = D3D11_SHADER_INPUT_BIND_DESC{};
		ThrowIfFailed(reflection->GetResourceBindingDesc(resourceIndex, &desc));
		const auto bindingDesc = ResourceBindingDesc{ desc };
		switch (bindingDesc.Type) {
		case D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER:
			numBuffers++;
			break;
		case D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE:
			numViews++;
			break;
		case D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER: {
			numSamplers++;
			break;
		}
		default:
			break;
		}
		bindingDescs.push_back(bindingDesc);
	}
}

const ConstantBufferDescs& ReflectiveShader::GetConstantBufferDescs() const noexcept
{
	return constantBufferDescs;
}

const ResourceBindingDescs& ReflectiveShader::GetResourceBindingDescs() const noexcept
{
	return bindingDescs;
}

void ReflectiveShader::Apply(ID3D11DeviceContext4* deviceContext,
	const ConstantBufferMap& constantBufferMap,
	const ShaderResourceViewMap& shaderResourceViewMap,
	const SamplerStateMap& samplerStateMap) noexcept
{
	for (const auto& desc : bindingDescs) {
		switch (desc.Type) {
		case D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER:
			constantBuffers[desc.BindPoint] = constantBufferMap.at(desc.Name);
			break;
		case D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE:
			shaderResourceViews[desc.BindPoint] = shaderResourceViewMap.at(desc.Name);
			break;
		case D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER:
			samplers[desc.BindPoint] = samplerStateMap.at(desc.Name);
			break;
		default:
			break;
		}
	}
}

VertexShader::VertexShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: BaseShader(graphicsDevice, shaderBytecode, bytecodeLength)
{
	ThrowIfFailed(graphicsDevice->CreateVertexShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

void VertexShader::Apply(ID3D11DeviceContext4* deviceContext,
	const ConstantBufferMap& constantBufferMap,
	const ShaderResourceViewMap& shaderResourceViewMap,
	const SamplerStateMap& samplerStateMap) noexcept
{
	ReflectiveShader::Apply(deviceContext, constantBufferMap, shaderResourceViewMap, samplerStateMap);

	deviceContext->VSSetShader(shader.Get(), NULL, 0);
	deviceContext->VSSetConstantBuffers(0, numBuffers, constantBuffers);
	deviceContext->VSSetShaderResources(0, numViews, shaderResourceViews);
	deviceContext->VSSetSamplers(0, numSamplers, samplers);
}

GeometryShader::GeometryShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: BaseShader(graphicsDevice, shaderBytecode, bytecodeLength)
{
	ThrowIfFailed(graphicsDevice->CreateGeometryShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

void GeometryShader::Apply(ID3D11DeviceContext4* deviceContext,
	const ConstantBufferMap& constantBufferMap,
	const ShaderResourceViewMap& shaderResourceViewMap,
	const SamplerStateMap& samplerStateMap) noexcept
{
	ReflectiveShader::Apply(deviceContext, constantBufferMap, shaderResourceViewMap, samplerStateMap);

	deviceContext->GSSetShader(shader.Get(), NULL, 0);
	deviceContext->GSSetConstantBuffers(0, numBuffers, constantBuffers);
	deviceContext->GSSetShaderResources(0, numViews, shaderResourceViews);
	deviceContext->GSSetSamplers(0, numSamplers, samplers);
}

PixelShader::PixelShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: BaseShader(graphicsDevice, shaderBytecode, bytecodeLength)
{
	ThrowIfFailed(graphicsDevice->CreatePixelShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

void PixelShader::Apply(ID3D11DeviceContext4* deviceContext,
	const ConstantBufferMap& constantBufferMap,
	const ShaderResourceViewMap& shaderResourceViewMap,
	const SamplerStateMap& samplerStateMap) noexcept
{
	ReflectiveShader::Apply(deviceContext, constantBufferMap, shaderResourceViewMap, samplerStateMap);

	deviceContext->PSSetShader(shader.Get(), NULL, 0);
	deviceContext->PSSetConstantBuffers(0, numBuffers, constantBuffers);
	deviceContext->PSSetShaderResources(0, numViews, shaderResourceViews);
	deviceContext->PSSetSamplers(0, numSamplers, samplers);
}

Shader::Shader(ID3D11Device5* graphicsDevice,
	const void* vertexShaderBytecode, size_t vertexShaderBytecodeLength,
	const void* geometryShaderBytecode, size_t geometryShaderBytecodeLength,
	const void* pixelShaderBytecode, size_t pixelShaderBytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	// 頂点、ジオメトリ、ピクセル シェーダーを作成
	shaders.reserve(3);
	shaders.push_back(std::make_unique<VertexShader>(
		graphicsDevice, vertexShaderBytecode, vertexShaderBytecodeLength));
	shaders.push_back(std::make_unique<GeometryShader>(
		graphicsDevice, geometryShaderBytecode, geometryShaderBytecodeLength));
	shaders.push_back(std::make_unique<PixelShader>(
		graphicsDevice, pixelShaderBytecode, pixelShaderBytecodeLength));

	// 各シェーダーからグローバル変数用の定数バッファーを検索
	for (const auto& shader : shaders) {
		std::unordered_set<std::string> descSet;

		// 定数バッファー
		for (auto& desc : shader->GetConstantBufferDescs()) {
			const auto& name = desc.Name;
			auto findItr = descSet.find(name);
			if (findItr == descSet.cend()) {
				descSet.insert(name);
				constantBufferDescs.push_back(desc);
			}
		}

		descSet.clear();

		// バインディング リソース
		for (auto& desc : shader->GetResourceBindingDescs()) {
			const auto name = desc.Name;
			auto findItr = descSet.find(name);
			if (findItr == descSet.cend()) {
				descSet.insert(name);
				bindingDescs.push_back(desc);
			}
		}
	}
}

const ConstantBufferDescs& Shader::GetConstantBufferDescs() const noexcept
{
	return constantBufferDescs;
}

const ResourceBindingDescs& Shader::GetResourceBindingDescs() const noexcept
{
	return bindingDescs;
}

void Shader::Apply(ID3D11DeviceContext4* deviceContext,
	const ConstantBufferMap& constantBufferMap,
	const ShaderResourceViewMap& shaderResourceViewMap,
	const SamplerStateMap& samplerStateMap) noexcept
{
	for (const auto& shader : shaders) {
		shader->Apply(deviceContext, constantBufferMap, shaderResourceViewMap, samplerStateMap);
	}
}
