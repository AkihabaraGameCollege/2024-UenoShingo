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
        
        float3 worldNormal = mul(input[i].normal.xyz, (float3x3) matrixWorld);
        worldNormal = normalize(worldNormal);
        element.worldNormal = float4(worldNormal, 0);
        
        element.position = mul(input[i].position, matrixWorldViewProjection);
        element.texCoord = input[i].texCoord;
        output.Append(element);
    }
}