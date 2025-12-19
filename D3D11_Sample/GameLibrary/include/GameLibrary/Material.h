#pragma once

#include <GameLibrary/Graphics.h>
#include <GameLibrary/Shader.h>
#include <GameLibrary/Buffer.h>
#include <GameLibrary/Texture.h>
#include <Windows.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

namespace GameLibrary
{
	class MaterialProperty
	{
	public:
		MaterialProperty(const std::string& name, ShaderPropertyType type) noexcept;
		virtual ~MaterialProperty() = default;

		const std::string& GetName() const noexcept;
		ShaderPropertyType GetType() const noexcept;

		virtual const DirectX::XMFLOAT4& GetColor() const;
		virtual const DirectX::XMFLOAT4& GetVector() const;
		virtual float GetFloat() const;
		virtual std::shared_ptr<Texture2D> GetTexture() const;
		virtual int GetInt() const;
		virtual const DirectX::XMFLOAT4X4& GetMatrix() const;
		virtual std::shared_ptr<ConstantBuffer> GetConstantBuffer() const;

		virtual void SetColor(const DirectX::XMFLOAT4& value) noexcept {}
		virtual void SetVector(const DirectX::XMFLOAT4& value) noexcept {}
		virtual void SetFloat(float value) noexcept {}
		virtual void SetTexture(std::shared_ptr<Texture2D> value) noexcept {}
		virtual void SetInt(int value) noexcept {}
		virtual void SetMatrix(const DirectX::XMFLOAT4X4& value) noexcept {}
		virtual void SetConstantBuffer(std::shared_ptr<ConstantBuffer> value) noexcept {}

	private:
		std::string name;
		ShaderPropertyType type = {};
	};

	class ConstantBufferProperty final : public MaterialProperty
	{
	public:
		ConstantBufferProperty(const ConstantBufferDesc& desc, ID3D11Device5* graphicsDevice) noexcept;

		const std::vector<BYTE>& GetConstants() const noexcept;
		std::shared_ptr<ConstantBuffer> GetConstantBuffer() const noexcept override;
		void SetConstant(const void* value, size_t startOffset, size_t size) noexcept;
		void SetConstantBuffer(std::shared_ptr<ConstantBuffer> value) noexcept override;

	private:
		std::vector<BYTE> constants;
		std::shared_ptr<ConstantBuffer> constantBuffer;
	};

	class VariableProperty : public MaterialProperty
	{
	public:
		VariableProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept;

		std::shared_ptr<ConstantBufferProperty> GetBuffer() const noexcept;

		void SetColor(const DirectX::XMFLOAT4& value) noexcept override;
		void SetVector(const DirectX::XMFLOAT4& value) noexcept override;
		void SetFloat(float value) noexcept override;
		void SetInt(int value) noexcept override;
		void SetMatrix(const DirectX::XMFLOAT4X4& value) noexcept override;

	private:
		std::shared_ptr<ConstantBufferProperty> buffer;
		size_t startOffset = {};
		size_t size = {};
	};

	class VectorProperty final : public VariableProperty
	{
	public:
		VectorProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept;

		const DirectX::XMFLOAT4& GetVector() const noexcept;
		void SetVector(const DirectX::XMFLOAT4& value) noexcept override;
		void SetColor(const DirectX::XMFLOAT4& value) noexcept override;

	private:
		DirectX::XMFLOAT4 data = {};
	};

	class FloatProperty final : public VariableProperty
	{
	public:
		FloatProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept;

		float GetFloat() const noexcept;
		void SetFloat(float value) noexcept override;

	private:
		float data = {};
	};

	class TextureProperty final : public MaterialProperty
	{
	public:
		TextureProperty(const ResourceBindingDesc& desc, const std::shared_ptr<Texture2D>& data) noexcept;

		std::shared_ptr<Texture2D> GetTexture() const noexcept;
		size_t GetSamplerNameId() const noexcept;
		void SetTexture(std::shared_ptr<Texture2D> value) noexcept override;

	private:
		std::shared_ptr<Texture2D> data;
		size_t samplerNameId = {};
	};

	class IntProperty final : public VariableProperty
	{
	public:
		IntProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept;

		int GetInt() const noexcept override;
		void SetInt(int value) noexcept override;

	private:
		int data = {};
	};

	class MatrixProperty final : public VariableProperty
	{
	public:
		MatrixProperty(const VariableDesc& desc, std::shared_ptr<ConstantBufferProperty> buffer) noexcept;

		const DirectX::XMFLOAT4X4& GetMatrix() const noexcept override;
		void SetMatrix(const DirectX::XMFLOAT4X4& value) noexcept override;

	private:
		DirectX::XMFLOAT4X4 data = {};
	};

	typedef std::unordered_map<size_t, std::shared_ptr<MaterialProperty>> PropertyMap;

	/// <summary>
	/// レンダリングで使用されるマテリアルを表します。
	/// </summary>
	class Material final : public GraphicsResource
	{
	public:
		Material(ID3D11Device5* graphicsDevice,
			std::shared_ptr<ConstantBufferManager> constantBufferManager,
			std::shared_ptr<Shader> shader);
		~Material() = default;

		std::shared_ptr<Shader> GetShader() noexcept;
		DirectX::XMFLOAT4 GetColor(size_t nameId) const noexcept;
		DirectX::XMFLOAT4 GetColor(const std::string& name) const noexcept;
		DirectX::XMFLOAT4 GetVector(size_t nameId) const noexcept;
		DirectX::XMFLOAT4 GetVector(const std::string& name) const noexcept;
		float GetFloat(size_t nameId) const noexcept;
		float GetFloat(const std::string& name) const noexcept;
		std::shared_ptr<Texture2D> GetTexture(size_t nameId) const noexcept;
		std::shared_ptr<Texture2D> GetTexture(const std::string& name) const noexcept;
		int GetInteger(size_t nameId) const noexcept;
		int GetInteger(const std::string& name) const noexcept;
		std::shared_ptr<ConstantBuffer> GetConstantBuffer(size_t nameId) const noexcept;
		std::shared_ptr<ConstantBuffer> GetConstantBuffer(const std::string& name) const noexcept;
		DirectX::XMFLOAT4X4 GetMatrix(size_t nameId) const noexcept;
		DirectX::XMFLOAT4X4 GetMatrix(const std::string& name) const noexcept;

		void SetShader(std::shared_ptr<Shader> value) noexcept;
		void SetColor(size_t nameId, const DirectX::XMFLOAT4& value) noexcept;
		void SetColor(const std::string& name, const DirectX::XMFLOAT4& value) noexcept;
		void SetVector(size_t nameId, const DirectX::XMFLOAT4& value) noexcept;
		void SetVector(const std::string& name, const DirectX::XMFLOAT4& value) noexcept;
		void SetFloat(size_t nameId, float value) noexcept;
		void SetFloat(const std::string& name, float value) noexcept;
		void SetTexture(size_t nameId, std::shared_ptr<Texture2D> value) noexcept;
		void SetTexture(const std::string& name, std::shared_ptr<Texture2D> value) noexcept;
		void SetInteger(size_t nameId, int value) noexcept;
		void SetInteger(const std::string& name, int value) noexcept;
		void SetConstantBuffer(size_t nameId, std::shared_ptr<ConstantBuffer> value) noexcept;
		void SetConstantBuffer(const std::string& name, std::shared_ptr<ConstantBuffer> value) noexcept;
		void SetMatrix(size_t nameId, const DirectX::XMFLOAT4X4& value) noexcept;
		void SetMatrix(const std::string& name, const DirectX::XMFLOAT4X4& value) noexcept;

		void Apply(ID3D11DeviceContext4* deviceContext) noexcept;

	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<ConstantBufferManager> constantBufferManager;
		PropertyMap propertyMap;
		ConstantBufferMap constantBufferMap;
		ShaderResourceViewMap shaderResourceViewMap;
		SamplerStateMap samplerStateMap;
	};
}
