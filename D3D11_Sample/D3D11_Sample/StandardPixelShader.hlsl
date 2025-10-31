#include "StandardShader.hlsli"

float4 main(PixelShaderInput input) : SV_TARGET
{
	// –Ê‚©‚çŒõŒ¹‚ğw‚·³‹K‰»ƒxƒNƒgƒ‹L
    float3 light = normalize(LightPosition.xyz - LightPosition.w * input.worldPosition.xyz);

    // ŠgU”½Ë
    float diffuse = max(dot(light, input.worldNormal), 0);
    float3 diffuseColor = diffuse * MaterialDiffuseColor.rgb;

    // ‹¾–Ê”½Ë
    float3 reflect = 2 * input.worldNormal * dot(input.worldNormal, light) - light;
    float3 viewDir = normalize(ViewPosition - input.worldPosition).xyz;
    float specular = pow(saturate(dot(reflect, viewDir)), MaterialSpecularPower);
    float3 specularColor = specular * MaterialSpecularColor.rgb;

    return float4(diffuseColor + specularColor, MaterialDiffuseColor.a);
}