#include "StandardShader.hlsli"

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    //output.position = input.position;
    //output.normal = input.normal;
    //output.texCoord = input.texCoord;
    output.position = input.position;
    output.color = input.color;
    return output;
}