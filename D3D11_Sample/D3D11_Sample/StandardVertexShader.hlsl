#include "StandardShader.hlsli"

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = input.position;
    output.normal = input.normal;
    return output;
}