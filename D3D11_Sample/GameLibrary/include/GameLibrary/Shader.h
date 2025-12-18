#pragma once

#include <GameLibrary/Graphics.h>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11_4.h>
#include <d3d11shader.h>

namespace GameLibrary
{
	typedef std::unordered_map<size_t, ID3D11Buffer*> ConstantBufferMap;
	typedef std::unordered_map<size_t, ID3D11ShaderResourceView*> ShaderResourceViewMap;
	typedef std::unordered_map<size_t, ID3D11SamplerState*> SamplerStateMap;

	enum class ShaderPropertyType
	{
		//Color,
		Vector,
		Float,
		Texture,
		Int,
		Matrix,
		ConstantBuffer,
		SamplerState,
	};

	// Variables
	struct VariableDesc
	{
		size_t NameId = 0;
		std::string Name;
		ShaderPropertyType Type = {};
		UINT StartOffset = 0;
		UINT Size = 0;
		UINT uFlags = 0;
		std::vector<BYTE> DefaultValue;
		UINT StartTexture = 0;
		UINT TextureSize = 0;
		UINT StartSampler = 0;
		UINT SamplerSize = 0;
		size_t ConstantBufferId = 0;

		explicit VariableDesc(ID3D11ShaderReflectionVariable* variable);
	};

	// ConstantBuffers
	struct ConstantBufferDesc
	{
		size_t NameId;
		std::string Name;
		D3D_CBUFFER_TYPE Type;
		std::vector<VariableDesc> Variables;
		UINT Size;
		UINT uFlags;

		explicit ConstantBufferDesc(ID3D11ShaderReflectionConstantBuffer* constantBuffer);
	};
	typedef std::vector<ConstantBufferDesc> ConstantBufferDescs;

	// ResourceBindings
	struct ResourceBindingDesc
	{
		size_t NameId;
		std::string Name;
		D3D_SHADER_INPUT_TYPE Type;
		UINT BindPoint;
		UINT BindCount;
		UINT uFlags;
		D3D_RESOURCE_RETURN_TYPE ReturnType;
		D3D_SRV_DIMENSION Dimension;
		UINT NumSamples;
		size_t SamplerNameId = 0;

		explicit ResourceBindingDesc(const D3D11_SHADER_INPUT_BIND_DESC& bindingDesc);
	};
	typedef std::vector<ResourceBindingDesc> ResourceBindingDescs;

	/// <summary>
	/// 定数バッファーとグローバル変数の定義を取得できるシェーダーを表します。
	/// </summary>
	class ReflectiveShader : public GraphicsResource
	{
	public:
		ReflectiveShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		virtual ~ReflectiveShader() = default;

		const ConstantBufferDescs& GetConstantBufferDescs() const noexcept;
		const ResourceBindingDescs& GetResourceBindingDescs() const noexcept;

		virtual void Apply(ID3D11DeviceContext4* deviceContext,
			const ConstantBufferMap& constantBufferMap,
			const ShaderResourceViewMap& shaderResourceViewMap,
			const SamplerStateMap& samplerStateMap) noexcept;

	protected:
		UINT numBuffers = 0;
		UINT numViews = 0;
		UINT numSamplers = 0;
		ID3D11Buffer* constantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = {};
		ID3D11ShaderResourceView* shaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
		ID3D11SamplerState* samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = {};

	private:
		ConstantBufferDescs constantBufferDescs;
		ResourceBindingDescs bindingDescs;
	};

	/// <summary>
	/// シェーダーのネイティブ ポインターを管理するクラスを作成するテンプレートを表します。
	/// </summary>
	/// <typeparam name="T">ネイティブ ポインターの型</typeparam>
	template<class T>
	class BaseShader : public ReflectiveShader
	{
	public:
		BaseShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		virtual ~BaseShader() = default;

		T* GetNativePointer() const noexcept;

	protected:
		Microsoft::WRL::ComPtr<T> shader;
	};

	template<class T>
	inline BaseShader<T>::BaseShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
		: ReflectiveShader(graphicsDevice, shaderBytecode, bytecodeLength)
	{

	}

	template<class T>
	inline T* BaseShader<T>::GetNativePointer() const noexcept
	{
		return shader.Get();
	}

	/// <summary>
	/// 頂点シェーダーを表します。
	/// </summary>
	class VertexShader final : public BaseShader<ID3D11VertexShader>
	{
	public:
		VertexShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~VertexShader() = default;

		void Apply(ID3D11DeviceContext4* deviceContext,
			const ConstantBufferMap& constantBufferMap,
			const ShaderResourceViewMap& shaderResourceViewMap,
			const SamplerStateMap& samplerStateMap) noexcept override;
	};

	/// <summary>
	/// ジオメトリ シェーダーを表します。
	/// </summary>
	class GeometryShader final : public BaseShader<ID3D11GeometryShader>
	{
	public:
		GeometryShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~GeometryShader() = default;

		void Apply(ID3D11DeviceContext4* deviceContext,
			const ConstantBufferMap& constantBufferMap,
			const ShaderResourceViewMap& shaderResourceViewMap,
			const SamplerStateMap& samplerStateMap) noexcept override;
	};

	/// <summary>
	/// ピクセル シェーダーを表します。
	/// </summary>
	class PixelShader final : public BaseShader<ID3D11PixelShader>
	{
	public:
		PixelShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~PixelShader() = default;

		void Apply(ID3D11DeviceContext4* deviceContext,
			const ConstantBufferMap& constantBufferMap,
			const ShaderResourceViewMap& shaderResourceViewMap,
			const SamplerStateMap& samplerStateMap) noexcept override;
	};

	/// <summary>
	/// 頂点、ジオメトリ、ピクセル シェーダーをまとめて管理する機能を提供します。
	/// </summary>
	class Shader : public GraphicsResource
	{
	public:
		Shader(ID3D11Device5* graphicsDevice,
			const void* vertexShaderBytecode, size_t vertexShaderBytecodeLength,
			const void* geometryShaderBytecode, size_t geometryShaderBytecodeLength,
			const void* pixelShaderBytecode, size_t pixelShaderBytecodeLength);

		static size_t StringNameToId(const std::string_view& name);

		const ConstantBufferDescs& GetConstantBufferDescs() const noexcept;
		const ResourceBindingDescs& GetResourceBindingDescs() const noexcept;

		void Apply(ID3D11DeviceContext4* deviceContext,
			const ConstantBufferMap& constantBufferMap,
			const ShaderResourceViewMap& shaderResourceViewMap,
			const SamplerStateMap& samplerStateMap) noexcept;

	private:
		std::vector<std::unique_ptr<ReflectiveShader>> shaders;

		ConstantBufferDescs constantBufferDescs;
		ResourceBindingDescs bindingDescs;
	};
}
