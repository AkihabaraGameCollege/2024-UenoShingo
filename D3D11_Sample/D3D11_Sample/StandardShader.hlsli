// 毎フレーム更新する定数バッファー
cbuffer ConstanBufferPerFrame
{
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4x4 WVP_Matrix;
    float4 MaterialColor;
};

// 頂点シェーダーへの入力
struct VertexShaderInput
{
    //float4 position : POSITION;
    //float4 normal : NORMAL;
    //float2 texCoord : TEXCOORD;
    float4 position : POSITION;
    float4 color : COLOR;
};

// 頂点シェーダーからの出力
struct VertexShaderOutput
{
    //float4 position : SV_POSITION;
    //float4 normal : NORMAL;
    //float2 texCoord : TEXCOORD;
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// ピクセル シェーダーへの入力
typedef VertexShaderOutput PixelShaderInput;
