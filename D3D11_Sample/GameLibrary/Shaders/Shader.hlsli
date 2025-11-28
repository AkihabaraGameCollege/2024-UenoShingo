struct VertexShaderInput_Sprite
{
	float4 position : POSITION;
};

struct VertexShaderOutput_Sprite
{
	float4 position : SV_POSITION;
};

typedef VertexShaderOutput_Sprite GeometryShaderInput_Sprite;

struct GeometryShaderOutput_Sprite
{
    float4 position : SV_POSITION;
};

typedef GeometryShaderOutput_Sprite PixelShaderInput_Sprite;
