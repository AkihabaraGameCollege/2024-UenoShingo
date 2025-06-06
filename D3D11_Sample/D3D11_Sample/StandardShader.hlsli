// 頂点シェーダーへの入力
struct VertexShaderInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

// 頂点シェーダーからの出力
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

// ピクセル シェーダーへの入力
typedef VertexShaderOutput PixelShaderInput;