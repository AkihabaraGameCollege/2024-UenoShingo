#include <GameLibrary/Texture.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

Texture2D::Texture2D(ID3D11Device5* graphicsDevice, UINT width, UINT height, DXGI_FORMAT format, const void* source)
	: GraphicsResource(graphicsDevice), width(width), height(height), format(format)
{
	// texture
	const auto textureDesc = D3D11_TEXTURE2D_DESC1{
		.Width = width,
		.Height = height,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = format,
		.SampleDesc = {.Count = 1, .Quality = 0, },
		.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE ,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
	};
	const auto initialData = D3D11_SUBRESOURCE_DATA{
		.pSysMem = source,
		.SysMemPitch = static_cast<UINT>(textureDesc.Width * sizeof(uint32_t)),
		.SysMemSlicePitch = 0,
	};
	ThrowIfFailed(graphicsDevice->CreateTexture2D1(&textureDesc, &initialData, &nativePointer));

	// shader resource view
	const auto viewDesc = D3D11_SHADER_RESOURCE_VIEW_DESC1{
		.Format = textureDesc.Format,
		.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D = {.MostDetailedMip = 0, .MipLevels = 1, .PlaneSlice = 0, },
	};
	ThrowIfFailed(graphicsDevice->CreateShaderResourceView1(nativePointer.Get(), &viewDesc, &view));

	// sampler state
	const auto samplerDesc = D3D11_SAMPLER_DESC{
		.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT,
		.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
		.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
		.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
		.MipLODBias = 0.0f,
		.MaxAnisotropy = 0,
		.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS,
		.BorderColor = { 0.0f, 0.0f, 0.0f, 0.0f },
		.MinLOD = 0,
		.MaxLOD = 0,
	};
	ThrowIfFailed(graphicsDevice->CreateSamplerState(&samplerDesc, &samplerState));
}

/// <summary>
/// テクスチャの幅を取得します。
/// </summary>
/// <returns>テクスチャの幅</returns>
UINT Texture2D::GetWidth() const noexcept
{
	return width;
}

/// <summary>
/// テクスチャの高さを取得します。
/// </summary>
/// <returns>テクスチャの高さ</returns>
UINT Texture2D::GetHeight() const noexcept
{
	return height;
}

/// <summary>
/// ID3D11Texture2D1 のネイティブ ポインターを取得します。
/// </summary>
/// <returns>ネイティブ ポインター</returns>
ID3D11Texture2D1* Texture2D::GetNativePointer() noexcept
{
	return nativePointer.Get();
}

/// <summary>
/// このテクスチャのシェーダーリソースビューを取得します。
/// </summary>
/// <returns>シェーダーリソースビュー</returns>
ID3D11ShaderResourceView1* Texture2D::GetView() noexcept
{
	return view.Get();
}

/// <summary>
/// このテクスチャのサンプラーを取得します。
/// </summary>
/// <returns>サンプラー ステート</returns>
ID3D11SamplerState* Texture2D::GetSamplerState() noexcept
{
	return samplerState.Get();
}
