#include "StandardShader.hlsli"

// ライトの入射ベクトル(平行光源 w = 0)
static const float4 lightPosition = float4(0.5f, 1.0f, -1.0f, 0.0f);
// マテリアルの表面カラー
static const float4 diffuseColor = float4(1, 0.8, 0.2, 1); // 色を強調
static const float3 ambientColor = float3(0.2, 0.2, 0.2); // 環境光
static const float3 specularColor = float3(1.0, 1.0, 1.0); // 鏡面反射色
static const float shininess = 16.0f; // ハイライトを広げる

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

        // ワールド空間上の法線ベクトルN
        float3 worldNormal = normalize(mul(input[i].normal.xyz, (float3x3) WorldMatrix));
        // ライトベクトル
        float3 light = normalize(lightPosition.xyz);

        // 視線ベクトル（カメラはz=-10にあるのでz方向）
        float3 viewDir = normalize(float3(0, 0, -10) - mul(input[i].position, WorldMatrix).xyz);

        // ランバート反射
        float diffuse = max(dot(light, worldNormal), 0);

        // 鏡面反射
        float3 reflectDir = reflect(-light, worldNormal);
        float spec = pow(max(dot(viewDir, reflectDir), 0), shininess);

        // 合成
        float3 color = ambientColor +
                       diffuse * diffuseColor.rgb +
                       spec * specularColor;

        element.color = float4(color, diffuseColor.a);

        output.Append(element);
    }
}