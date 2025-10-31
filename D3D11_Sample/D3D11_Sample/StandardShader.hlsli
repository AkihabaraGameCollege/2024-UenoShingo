// 毎フレーム更新する定数バッファー
cbuffer ConstanBufferPerFrame
{
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4x4 WVP_Matrix;
    float4 ViewPosition;
    float4 LightPosition;
    float4 MaterialDiffuseColor;
    float3 MaterialSpecularColor;
    float MaterialSpecularPower;
};

// 頂点シェーダーへの入力
struct VertexShaderInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};

// 頂点シェーダーからの出力
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

// ジオメトリー シェーダーへの入力
typedef VertexShaderOutput GeometryShaderInput;

// ジオメトリー シェーダーからの出力
struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float3 worldNormal : NORMAL;
};

// ピクセル シェーダーへの入力
typedef GeometryShaderOutput PixelShaderInput;
