cbuffer ConstantBufferPerLighting
{
    float4 lightDirection;
    float4 lightColor;
};

cbuffer ConstantBufferPerDraw
{
    matrix matrixWorld;
};

cbuffer ConstantBufferPerFrame
{
    matrix matrixView;
    matrix matrixProjection;
    matrix matrixViewProjection;
};

static const matrix matrixWorldViewProjection = mul(matrixWorld, matrixViewProjection);

struct VertexShaderInput_Base
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput_Base
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

typedef VertexShaderOutput_Base GeometryShaderInput_Base;

struct GeometryShaderOutput_Base
{
    float4 worldPosition : POSITION;
    float4 worldNormal : NORMAL;
    float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

typedef GeometryShaderOutput_Base PixelShaderInput_Base;

struct VertexShaderInput_Sprite
{
	float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput_Sprite
{
	float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

typedef VertexShaderOutput_Sprite GeometryShaderInput_Sprite;

struct GeometryShaderOutput_Sprite
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

typedef GeometryShaderOutput_Sprite PixelShaderInput_Sprite;
