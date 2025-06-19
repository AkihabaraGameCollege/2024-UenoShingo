#include "StandardShader.hlsli"

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, WorldMatrix);
    output.position = mul(output.position, ViewMatrix);
    
	// 正射影(Orthographic Projection)
	// 遠近法を使用せずに射影する方法。
	// widthとheightは自由に設定できるが、ほとんどのケースでスクリーン画面の
	// 幅と高さの比率(アスペクト比 = 今回は4:3)で指定する。
    const float width = 4; // クリップ面の幅
    const float height = 3; // クリップ面の高さ
    const float near = 0.3f; // nearクリップ面
    const float far = 1000.0f; // farクリップ面
	// 幅と高さを1/2しているのはposition.x、yを[-1.0f, 1.0f]の範囲に整えるため。
    output.position.x = output.position.x / (width / 2);
    output.position.y = output.position.y / (height / 2);
    output.position.z = (output.position.z - near) / (far - near);
    output.position.w = output.position.w;
	/*
	// 行列で計算する場合
	const float4x4 Projection = {
		 2 / width,       0.0f,                 0.0f, 0.0f,
			  0.0f, 2 / height,                 0.0f, 0.0f,
			  0.0f,       0.0f,     1 / (far - near), 0.0f,
			  0.0f,       0.0f, -near / (far - near), 1.0f,
	};
	output.position = mul(output.position, Projection);
	*/
    
    output.color = input.color;
    return output;
}