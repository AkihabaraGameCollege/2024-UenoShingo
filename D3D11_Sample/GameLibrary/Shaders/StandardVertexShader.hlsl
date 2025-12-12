#include "StandardShader.hlsli"

VertexShaderOutput_Base main(VertexShaderInput_Base input)
{
    VertexShaderOutput_Base output;
    output.position = input.position;
    output.normal = input.normal;
    output.texCoord = input.texCoord;
    return output;
}
