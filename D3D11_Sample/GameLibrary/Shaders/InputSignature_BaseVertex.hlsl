#include "Shader.hlsli"

float4 main(VertexShaderInput_Base input) : SV_POSITION
{
    return input.position;
}
