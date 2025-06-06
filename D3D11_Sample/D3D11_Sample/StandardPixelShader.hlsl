#include "StandardShader.hlsli"

float4 main(PixelShaderInput input) : SV_TARGET
{
    return input.color;
}