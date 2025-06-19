#include "StandardShader.hlsli"

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, WorldMatrix);
    output.position = mul(output.position, ViewMatrix);
    
	// プロジェクション変換後のz座標が[0.0f, 1.0f]の範囲となるようにスケーリング
    const float near = 0.3f; // nearクリップ面
    const float far = 1000.0f; // farクリップ面
    output.position.x = output.position.x;
    output.position.y = output.position.y;
    output.position.z = (output.position.z - near) / (far - near);
    output.position.w = output.position.w;
	/*
	// 行列で計算する場合
	//output.position.z = output.position.z / (far - near) - near / (far - near);
	const float4x4 Projection = {
		1.0f, 0.0f,                 0.0f, 0.0f,
		0.0f, 1.0f,                 0.0f, 0.0f,
		0.0f, 0.0f,     1 / (far - near), 0.0f,
		0.0f, 0.0f, -near / (far - near), 1.0f,
	};
	output.position = mul(output.position, Projection);
	*/
    
    output.color = input.color;
    return output;
}