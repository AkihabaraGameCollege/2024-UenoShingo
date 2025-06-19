#include "StandardShader.hlsli"

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, WorldMatrix);
    output.position = mul(output.position, ViewMatrix);
    
	// nearクリップ面
	// プロジェクション変換後はz座標が[0.0f, 1.0f]の範囲だけを表示するため、
	// ここではnearをオブジェクトのz座標(10.0f)に近づけている
    const float near = 9.5f;
	// nearクリップ面が原点になるように平行移動変換
    output.position.x = output.position.x;
    output.position.y = output.position.y;
    output.position.z = output.position.z - near;
    output.position.w = output.position.w;
	/*
	// 行列で計算する場合
	const float4x4 Projection = {
		1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 1.0f,  0.0f, 0.0f,
		0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f, -near, 1.0f,
	};
	output.position = mul(output.position, Projection);
	*/
    
    output.color = input.color;
    return output;
}