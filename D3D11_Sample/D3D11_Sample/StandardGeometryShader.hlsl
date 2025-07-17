#include "StandardShader.hlsli"

// ライトの入射ベクトル(平行光源 w = 0)
static const float4 lightPosition = float4(1.0f, 2.0f, -2.0f, 0.0f);
//// ライトの位置座標(点光源 w = 1)
//static const float4 lightPosition = float4(1.0f, 2.0f, -2.0f, 1.0f);
// マテリアルの表面カラー
static const float4 diffuseColor = float4(1, 1, 0, 1);

[maxvertexcount(3)]
void main(
	triangle GeometryShaderInput input[3],
	inout TriangleStream<GeometryShaderOutput> output)
{
    [unroll]
    for (uint i = 0; i < 3; i++)
    {
        GeometryShaderOutput element;
		// WVP変換
        element.position = mul(input[i].position, WVP_Matrix);

		// 面から光源を指す正規化ベクトルL(平行光源 w = 0)
        float3 light = normalize(lightPosition.xyz);

		// ワールド空間上の法線ベクトルN
        float3 worldNormal = normalize(mul(input[i].normal.xyz, (float3x3) WorldMatrix));
		// ランバート反射
        float diffuse = max(dot(light, worldNormal), 0);
        element.color = float4(diffuse * diffuseColor.rgb, diffuseColor.a);
        
        output.Append(element);
    }
}