#include <GameLibrary/Material.h>
#include <system_error>

using namespace GameLibrary;
using namespace DirectX;

MaterialProperty::MaterialProperty(const std::string& name, ShaderPropertyType type) noexcept
	: name(name), type(type)
{

}

const std::string& MaterialProperty::GetName() const noexcept
{
	return name;
}

ShaderPropertyType MaterialProperty::GetType() const noexcept
{
	return type;
}

const XMFLOAT4& MaterialProperty::GetColor() const
{
	throw std::system_error(std::make_error_code(std::errc::not_supported));
}

const DirectX::XMFLOAT4& MaterialProperty::GetVector() const
{
	throw std::system_error(std::make_error_code(std::errc::not_supported));
}

float MaterialProperty::GetFloat() const
{
	throw std::system_error(std::make_error_code(std::errc::not_supported));
}

std::shared_ptr<Texture2D> MaterialProperty::GetTexture() const
{
	throw std::system_error(std::make_error_code(std::errc::not_supported));
}

int MaterialProperty::GetInt() const
{
	throw std::system_error(std::make_error_code(std::errc::not_supported));
}

const DirectX::XMFLOAT4X4& MaterialProperty::GetMatrix() const
{
	throw std::system_error(std::make_error_code(std::errc::not_supported));
}

std::shared_ptr<ConstantBuffer> MaterialProperty::GetConstantBuffer() const
{
	throw std::system_error(std::make_error_code(std::errc::not_supported));
}

ConstantBufferProperty::ConstantBufferProperty(const ConstantBufferDesc& desc, ID3D11Device5* graphicsDevice) noexcept
	: MaterialProperty(desc.Name, ShaderPropertyType::ConstantBuffer)
{
	constants.resize(desc.Size, 0);
	constantBuffer = std::make_shared<ConstantBuffer>(graphicsDevice, desc.Size);
}

const std::vector<BYTE>& ConstantBufferProperty::GetConstants() const noexcept
{
	return constants;
}

std::shared_ptr<ConstantBuffer> ConstantBufferProperty::GetConstantBuffer() const noexcept
{
	return constantBuffer;
}

void ConstantBufferProperty::SetConstant(const void* value, size_t startOffset, size_t size) noexcept
{
	std::memcpy(constants.data() + startOffset, value, size);
}

void ConstantBufferProperty::SetConstantBuffer(std::shared_ptr<ConstantBuffer> value) noexcept
{
	constantBuffer = value;
}

VariableProperty::VariableProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept
	: MaterialProperty(desc.Name, desc.Type)
	, buffer(buffer), startOffset(desc.StartOffset), size(desc.Size)
{
	if (!desc.DefaultValue.empty()) {
		buffer->SetConstant(desc.DefaultValue.data(), desc.StartOffset, desc.Size);
	}
}

std::shared_ptr<ConstantBufferProperty> VariableProperty::GetBuffer() const noexcept
{
	return buffer;
}

void VariableProperty::SetColor(const DirectX::XMFLOAT4& value) noexcept
{
	// リニア色空間に変換した値をシェーダーに転送する
	auto linearColor = XMFLOAT4{};
	XMStoreFloat4(&linearColor, XMColorSRGBToRGB(XMLoadFloat4(&value)));
	buffer->SetConstant(&linearColor, startOffset, size);
}

void VariableProperty::SetVector(const DirectX::XMFLOAT4& value) noexcept
{
	buffer->SetConstant(&value, startOffset, size);
}

void VariableProperty::SetFloat(float value) noexcept
{
	buffer->SetConstant(&value, startOffset, size);
}

void VariableProperty::SetInt(int value) noexcept
{
	buffer->SetConstant(&value, startOffset, size);
}

void VariableProperty::SetMatrix(const DirectX::XMFLOAT4X4& value) noexcept
{
	buffer->SetConstant(&value, startOffset, size);
}

VectorProperty::VectorProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept
	: VariableProperty(desc, buffer)
{

}

const XMFLOAT4& VectorProperty::GetVector() const noexcept
{
	return data;
}

void VectorProperty::SetColor(const DirectX::XMFLOAT4& value) noexcept
{
	VariableProperty::SetColor(value);
	data = value;
}

void VectorProperty::SetVector(const DirectX::XMFLOAT4& value) noexcept
{
	VariableProperty::SetVector(value);
	data = value;
}

FloatProperty::FloatProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept
	: VariableProperty(desc, buffer)
{

}

float FloatProperty::GetFloat() const noexcept
{
	return data;
}

void FloatProperty::SetFloat(float value) noexcept
{
	VariableProperty::SetFloat(value);
	data = value;
}

TextureProperty::TextureProperty(const ResourceBindingDesc& desc, const std::shared_ptr<Texture2D>& data) noexcept
	: MaterialProperty(desc.Name, ShaderPropertyType::Texture)
	, data(data), samplerNameId(desc.SamplerNameId)
{

}

std::shared_ptr<Texture2D> TextureProperty::GetTexture() const noexcept
{
	return data;
}

size_t TextureProperty::GetSamplerNameId() const noexcept
{
	return samplerNameId;
}

void TextureProperty::SetTexture(std::shared_ptr<Texture2D> value) noexcept
{
	data = value;
}

IntProperty::IntProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept
	: VariableProperty(desc, buffer)
{

}

int IntProperty::GetInt() const noexcept
{
	return data;
}

void IntProperty::SetInt(int value) noexcept
{
	VariableProperty::SetInt(value);
	data = value;
}

MatrixProperty::MatrixProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept
	: VariableProperty(desc, buffer)
{

}

const XMFLOAT4X4& MatrixProperty::GetMatrix() const noexcept
{
	return data;
}

void MatrixProperty::SetMatrix(const XMFLOAT4X4& value) noexcept
{
	VariableProperty::SetMatrix(value);
	data = value;
}

Material::Material(ID3D11Device5* graphicsDevice,
	std::shared_ptr<ConstantBufferManager> constantBufferManager,
	std::shared_ptr<Shader> shader)
	: GraphicsResource(graphicsDevice)
	, constantBufferManager(constantBufferManager)
{
	SetShader(shader);
}

std::shared_ptr<Shader> Material::GetShader() noexcept
{
	return shader;
}

XMFLOAT4 Material::GetColor(size_t nameId) const noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return XMFLOAT4{};
	}
	const auto property = std::static_pointer_cast<const VectorProperty>(itr->second);
	return property->GetColor();
}

XMFLOAT4 Material::GetColor(const std::string& name) const noexcept
{
	return GetColor(Shader::StringNameToId(name));
}

XMFLOAT4 Material::GetVector(size_t nameId) const noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return XMFLOAT4{};
	}
	const auto property = std::static_pointer_cast<VectorProperty>(itr->second);
	return property->GetVector();
}

XMFLOAT4 Material::GetVector(const std::string& name) const noexcept
{
	return GetVector(Shader::StringNameToId(name));
}

float Material::GetFloat(size_t nameId) const noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return float{};
	}
	const auto property = std::static_pointer_cast<const FloatProperty>(itr->second);
	return property->GetFloat();
}

float Material::GetFloat(const std::string& name) const noexcept
{
	return GetFloat(Shader::StringNameToId(name));
}

std::shared_ptr<Texture2D> Material::GetTexture(size_t nameId) const noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return nullptr;
	}
	const auto property = std::static_pointer_cast<TextureProperty>(itr->second);
	return property->GetTexture();
}

std::shared_ptr<Texture2D> Material::GetTexture(const std::string& name) const noexcept
{
	return GetTexture(Shader::StringNameToId(name));
}

int Material::GetInteger(size_t nameId) const noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return int{};
	}
	const auto property = std::static_pointer_cast<const IntProperty>(itr->second);
	return property->GetInt();
}

int Material::GetInteger(const std::string& name) const noexcept
{
	return GetInteger(Shader::StringNameToId(name));
}

std::shared_ptr<ConstantBuffer> Material::GetConstantBuffer(size_t nameId) const noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return nullptr;
	}
	const auto property = std::static_pointer_cast<const ConstantBufferProperty>(itr->second);
	return property->GetConstantBuffer();
}

std::shared_ptr<ConstantBuffer> Material::GetConstantBuffer(const std::string& name) const noexcept
{
	return GetConstantBuffer(Shader::StringNameToId(name));
}

XMFLOAT4X4 Material::GetMatrix(size_t nameId) const noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return XMFLOAT4X4{};
	}
	const auto property = std::static_pointer_cast<const IntProperty>(itr->second);
	return property->GetMatrix();
}

XMFLOAT4X4 Material::GetMatrix(const std::string& name) const noexcept
{
	return GetMatrix(Shader::StringNameToId(name));
}

void Material::SetShader(std::shared_ptr<Shader> value) noexcept
{
	shader = value;

	// 定数バッファーを検索
	const auto& bufferDescs = shader->GetConstantBufferDescs();
	constantBufferMap.clear();
	constantBufferMap.reserve(bufferDescs.size());
	for (auto& bufferDesc : bufferDescs) {
		const auto nameId = bufferDesc.NameId;

		// システム標準の定数バッファーの場合
		const auto findBuffer = constantBufferManager->Find(nameId);
		if (findBuffer != nullptr) {
			constantBufferMap.insert(std::make_pair(nameId, findBuffer->GetNativePointer()));
			continue;
		}

		// 定数バッファーがプロパティとして登録済み
		auto findItr = propertyMap.find(nameId);
		if (findItr != propertyMap.cend()) {
			const auto property = static_pointer_cast<ConstantBufferProperty>(findItr->second);
			const auto constantBuffer = property->GetConstantBuffer();
			if (constantBuffer == nullptr) {
				constantBufferMap.insert(std::make_pair(nameId, nullptr));
			}
			else {
				constantBufferMap.insert(std::make_pair(nameId, constantBuffer->GetNativePointer()));
			}
			continue;
		}

		// 新しい定数バッファーとしてプロパティを作成登録
		auto bufferProperty = std::make_shared<ConstantBufferProperty>(bufferDesc, GetDevice());
		propertyMap.insert(std::make_pair(nameId, bufferProperty));

		// グローバル変数を登録
		auto& variableDescs = bufferDesc.Variables;
		for (auto& variableDesc : variableDescs) {
			switch (variableDesc.Type) {
			case ShaderPropertyType::Vector:
				propertyMap.insert(std::make_pair(variableDesc.NameId,
					std::make_unique<VectorProperty>(variableDesc, bufferProperty)));
				break;
			case ShaderPropertyType::Float:
				propertyMap.insert(std::make_pair(variableDesc.NameId,
					std::make_unique<FloatProperty>(variableDesc, bufferProperty)));
				break;
			case ShaderPropertyType::Texture:
				break;
			case ShaderPropertyType::Int:
				propertyMap.insert(std::make_pair(variableDesc.NameId,
					std::make_unique<IntProperty>(variableDesc, bufferProperty)));
				break;
			case ShaderPropertyType::Matrix:
				propertyMap.insert(std::make_pair(variableDesc.NameId,
					std::make_unique<MatrixProperty>(variableDesc, bufferProperty)));
				break;
			default:
				break;
			}
		}
		constantBufferMap.insert(std::make_pair(nameId, bufferProperty->GetConstantBuffer()->GetNativePointer()));
	}

	// バインディング リソースを検索
	shaderResourceViewMap.clear();
	samplerStateMap.clear();
	for (auto& desc : shader->GetResourceBindingDescs()) {
		if (desc.Type != D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE) {
			continue;
		}

		const auto nameId = desc.NameId;
		const auto samplerNameId = desc.SamplerNameId;

		auto propertyItr = propertyMap.find(nameId);
		if (propertyItr == propertyMap.cend()) {
			propertyMap.insert(std::make_pair(nameId, std::make_shared<TextureProperty>(desc, nullptr)));
			shaderResourceViewMap.insert(std::make_pair(nameId, nullptr));
			samplerStateMap.insert(std::make_pair(samplerNameId, nullptr));
			continue;
		}

		const auto property = static_pointer_cast<const TextureProperty>(propertyItr->second);
		auto texture = property->GetTexture();
		if (texture == nullptr) {
			shaderResourceViewMap.insert(std::make_pair(nameId, nullptr));
			samplerStateMap.insert(std::make_pair(samplerNameId, nullptr));
			continue;
		}

		shaderResourceViewMap.insert(std::make_pair(nameId, texture->GetView()));
		samplerStateMap.insert(std::make_pair(samplerNameId, texture->GetSamplerState()));
	}
}

void Material::SetColor(size_t nameId, const XMFLOAT4& value) noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return;
	}
	const auto property = std::static_pointer_cast<VectorProperty>(itr->second);
	property->SetColor(value);
}

void Material::SetColor(const std::string& name, const XMFLOAT4& value) noexcept
{
	SetColor(Shader::StringNameToId(name), value);
}

void Material::SetVector(size_t nameId, const XMFLOAT4& value) noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return;
	}
	const auto property = std::static_pointer_cast<VectorProperty>(itr->second);
	property->SetVector(value);
}

void Material::SetVector(const std::string& name, const XMFLOAT4& value) noexcept
{
	SetVector(Shader::StringNameToId(name), value);
}

void Material::SetFloat(size_t nameId, float value) noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return;
	}
	const auto property = std::static_pointer_cast<FloatProperty>(itr->second);
	property->SetFloat(value);
}

void Material::SetFloat(const std::string& name, float value) noexcept
{
	SetFloat(Shader::StringNameToId(name), value);
}

void Material::SetTexture(size_t nameId, std::shared_ptr<Texture2D> value) noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return;
	}
	const auto property = static_pointer_cast<TextureProperty>(itr->second);
	property->SetTexture(value);
	shaderResourceViewMap[nameId] = value->GetView();
	samplerStateMap[property->GetSamplerNameId()] = value->GetSamplerState();
}

void Material::SetTexture(const std::string& name, std::shared_ptr<Texture2D> value) noexcept
{
	SetTexture(Shader::StringNameToId(name), value);
}

void Material::SetInteger(size_t nameId, int value) noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return;
	}
	const auto property = std::static_pointer_cast<IntProperty>(itr->second);
	property->SetInt(value);
}

void Material::SetInteger(const std::string& name, int value) noexcept
{
	SetInteger(Shader::StringNameToId(name), value);
}

void Material::SetConstantBuffer(size_t nameId, std::shared_ptr<ConstantBuffer> value) noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return;
	}
	const auto property = std::static_pointer_cast<ConstantBufferProperty>(itr->second);
	property->SetConstantBuffer(value);
	constantBufferMap[nameId] = value->GetNativePointer();
}

void Material::SetConstantBuffer(const std::string& name, std::shared_ptr<ConstantBuffer> value) noexcept
{
	SetConstantBuffer(Shader::StringNameToId(name), value);
}

void Material::SetMatrix(size_t nameId, const XMFLOAT4X4& value) noexcept
{
	const auto itr = propertyMap.find(nameId);
	if (itr == propertyMap.cend()) {
		return;
	}
	const auto property = std::static_pointer_cast<MatrixProperty>(itr->second);
	property->SetMatrix(value);
}

void Material::SetMatrix(const std::string& name, const XMFLOAT4X4& value) noexcept
{
	SetMatrix(Shader::StringNameToId(name), value);
}

void Material::Apply(ID3D11DeviceContext4* deviceContext) noexcept
{
	for (const auto& pair : propertyMap) {
		if (pair.second->GetType() != ShaderPropertyType::ConstantBuffer) {
			continue;
		}
		const auto property = static_pointer_cast<ConstantBufferProperty>(pair.second);
		auto& constants = property->GetConstants();
		auto constantBuffer = property->GetConstantBuffer();
		constantBuffer->UpdateSubresource(constants.data());
	}
	shader->Apply(deviceContext, constantBufferMap, shaderResourceViewMap, samplerStateMap);
}
