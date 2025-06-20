#include "StandardShader.hlsli"

// これらはすべて定数であり、すべての頂点で固定値となる
static const float aspect = 480.0f / 640.0f; // スクリーン画面のアスペクト比
static const float near = 0.3f; // nearクリップ面
static const float far = 1000.0f; // farクリップ面
static const float fovY = radians(60); // 視錐台の垂直方向の角度（Field Of View）

VertexShaderOutput main(VertexShaderInput input)
{
    const float4 worldPosition = mul(input.position, WorldMatrix);
    const float4 viewPosition = mul(worldPosition, ViewMatrix);

    float4 position; // プロジェクション変換後の位置座標

	//// 【Step.1】
	//position.x = viewPosition.x * aspect / tan(fovY / 2) / viewPosition.z;
	//position.y = viewPosition.y / tan(fovY / 2) / viewPosition.z;
	//// プロジェクション空間の座標はピクセルシェーダーに渡される前に
	//// (x, y, z, w) => (x / w, y / w, z / w, 1)に変換される。
	//// w座標にビュー空間のz座標を設定しておくことで、プロジェクション変換後に
	//// xyz座標を定数ではないビュー空間のz座標によって除算できる。
	//// ただし、この方法はxとy座標についてはスマートな方法なのだがz座標まで
	//// 除算されることが問題となる。（範囲が[0.0f, 1.0f]ではなくなる）
	//// そこで先にz軸についてビュー空間のz座標で割り算しても、値が[0.0f, 1.0f]の
	//// 範囲となるように考えてみる。

	//// z軸についてはnearとfarのクリップ面の間で、すでに[0.0f, 1.0f]の範囲に
	//// スケーリングされている。   ⇒ (viewPosition.z - near) / (far - near)
	////
	//// このため追加でビュー空間のz座標で除算すると、プロジェクション変換後の
	//// z座標は[0.0f, 1.0f]の範囲に収まらなくなる。
	//// この問題に対処するために、ビュー空間のz座標をfarで除算して[0.0f, 1.0f]の
	//// 範囲に収めたパラメーターを考える。         ⇒ (viewPosition.z / far)
	////
	//// ともに[0.0f, 1.0f]の範囲にスケーリングされた値同士の除算は、
	//// その計算結果も[0.0f, 1.0f]の範囲に収まる。
	//// 以下の式でviewPosition.zがnearの場合は0.0f、farの場合は1.0fとなる。
	//position.z = (viewPosition.z - near) / (far - near) / (viewPosition.z / far);
	//position.w = viewPosition.w;

	//// 【Step.2】
	//position.x = viewPosition.x * aspect / tan(fovY / 2) / viewPosition.z;
	//position.y = viewPosition.y / tan(fovY / 2) / viewPosition.z;
	//position.z = (viewPosition.z - near) / (far - near) * far / viewPosition.z;
	//position.w = viewPosition.w;

	//// 【Step.3】
	//position.x = viewPosition.x * aspect / tan(fovY / 2);
	//position.y = viewPosition.y / tan(fovY / 2);
	//position.z = (viewPosition.z - near) / (far - near) * far;
	//position.x /= viewPosition.z;
	//position.y /= viewPosition.z;
	//position.z /= viewPosition.z;
	//position.w = viewPosition.w;

	//// 【Step.4】
	//position.x = viewPosition.x * aspect / tan(fovY / 2);
	//position.y = viewPosition.y / tan(fovY / 2);
	//position.z = (viewPosition.z - near) / (far - near) * far;
	//// w座標にビュー空間のz座標を設定
	//position.w = viewPosition.z;

	//// 【Step.5】
	//const float4x4 Projection = {
	//	aspect / tan(fovY / 2),              0.0f,                       0.0f, 0.0f,
	//	                  0.0f, 1 / tan(fovY / 2),                       0.0f, 0.0f,
	//	                  0.0f,              0.0f,     1 / (far - near) * far, 0.0f,
	//	                  0.0f,              0.0f, -near / (far - near) * far, 1.0f,
	//};
	//position = mul(viewPosition, Projection);
	//position.w = viewPosition.z;

	// 【Step.6】 パースペクティブ射影変換行列の完成
	// この行列はすべて定数で表現されているため、すべての頂点で毎回計算しなくても
	// 事前に用意しておくことができる。
    const float4x4 Projection =
    {
        aspect / tan(fovY / 2), 0.0f, 0.0f, 0.0f,
		                  0.0f, 1 / tan(fovY / 2), 0.0f, 0.0f,
		                  0.0f, 0.0f, 1 / (far - near) * far, 1.0f, // _34 = 1, _44 = 0によって
		                  0.0f, 0.0f, -near / (far - near) * far, 0.0f, // w座標にz座標がセットされる
    };
    position = mul(viewPosition, Projection);
    
    VertexShaderOutput output;
    output.position = position;
    output.color = input.color;
    return output;
}