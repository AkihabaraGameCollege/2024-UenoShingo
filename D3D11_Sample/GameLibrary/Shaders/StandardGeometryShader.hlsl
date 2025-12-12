#include "StandardShader.hlsli"

[maxvertexcount(3)]
void main(
	triangle GeometryShaderInput_Base input[3],
	inout TriangleStream<GeometryShaderOutput_Base> output)
{
	[unroll]
    for (uint i = 0; i < 3; i++)
    {
        GeometryShaderOutput_Base element;
        element.worldPosition = mul(input[i].position, matrixWorld);
        element.worldNormal = mul(input[i].normal, matrixWorld);
        element.position = mul(input[i].position, matrixWorldViewProjection);
        element.texCoord = input[i].texCoord;
        output.Append(element);
    }
}
