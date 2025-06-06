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

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = input.position;
    output.normal = input.normal;
    output.texCoord = input.texCoord;
    return output;
}