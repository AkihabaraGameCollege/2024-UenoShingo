#include "StandardShader.hlsli"

float4 main(PixelShaderInput_Base input) : SV_TARGET
{
    float4 outputColor = AlbedoColor * MainTexture.Sample(MainTextureSampler, input.texCoord);
    float diffuse = dot(-lightDirection, input.worldNormal);
    outputColor.rgb *= diffuse * lightColor.rgb;
    return outputColor;
}
