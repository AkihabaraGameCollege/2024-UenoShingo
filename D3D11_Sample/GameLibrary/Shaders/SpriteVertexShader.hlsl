#include "SpriteShader.hlsli"

VertexShaderOutput_Sprite main(VertexShaderInput_Sprite input)
{
	VertexShaderOutput_Sprite output;
	output.position = input.position;
	return output;
}
