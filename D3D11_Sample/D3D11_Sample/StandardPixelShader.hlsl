// ピクセル シェーダーへの入力
struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
    return float4(input.texCoord.x, input.texCoord.y, 0.0f, 1.0f);
}