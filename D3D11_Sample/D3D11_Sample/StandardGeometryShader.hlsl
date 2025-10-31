#include "StandardShader.hlsli"

[maxvertexcount(3)]
void main(
	triangle GeometryShaderInput input[3],
	inout TriangleStream<GeometryShaderOutput> output)
{
    [unroll]
    for (uint i = 0; i < 3; i++)
    {
        GeometryShaderOutput element;
        element.position = mul(input[i].position, WVP_Matrix);
        element.worldPosition = mul(input[i].position, WorldMatrix);
        element.worldNormal = mul(input[i].normal, (float3x3) WorldMatrix);
        output.Append(element);
    }
}