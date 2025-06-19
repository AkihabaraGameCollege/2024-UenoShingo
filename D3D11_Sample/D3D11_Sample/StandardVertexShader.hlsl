#include "StandardShader.hlsli"

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, WorldMatrix);
    output.position = mul(output.position, ViewMatrix);
    
	//// クリップ面のy座標の範囲を[-1.0f, 1.0f]として、x座標がアスペクト比を
	//// 維持するようにスケーリング
	//const float screenWidth = 640;	// スクリーン画面の幅
	//const float screenHeight = 480;	// スクリーン画面の高さ
	//const float aspect = screenHeight / screenWidth;	// スクリーン画面のアスペクト比
	//const float near = 0.3f;	// nearクリップ面
	//const float far = 1000.0f;	// farクリップ面
	//output.position.x = output.position.x * aspect;
	//output.position.y = output.position.y;
	//output.position.z = (output.position.z - near) / (far - near);
	//output.position.w = output.position.w;
	///*
	//// 行列で計算する場合
	////output.position.z = output.position.z / (far - near) - near / (far - near);
	//const float4x4 Projection = {
	//	aspect, 0.0f,                 0.0f, 0.0f,
	//	  0.0f, 1.0f,                 0.0f, 0.0f,
	//	  0.0f, 0.0f,     1 / (far - near), 0.0f,
	//	  0.0f, 0.0f, -near / (far - near), 1.0f,
	//};
	//output.position = mul(output.position, Projection);
	//*/

	// ビュー空間のz座標で除算することで、カメラから近い頂点は拡大され遠いものは縮小される。
	// このとき単純にz座標で除算するとy座標が[-1.0f, 1.0f]の範囲に収まらなくなる。
	// そこで視錐台(Viewing Frustum)の垂直方向の角度(Field Of View)を指定して、その範囲内が
	// 描画されるようにスケーリングする。
    const float aspect = 480.0f / 640.0f; // スクリーン画面のアスペクト比
    const float near = 0.3f; // nearクリップ面
    const float far = 1000.0f; // farクリップ面
	// 視錐台の垂直方向の角度(Field Of View)
    const float fovY = radians(60);
	// position.zの位置での視錐台上端のy座標
    const float frustumTop = tan(fovY / 2) * output.position.z;
	// position.y座標をfrustumTopで除算することにより[-1.0f, 1.0f]の範囲にスケーリングする。
    output.position.x = output.position.x * aspect / frustumTop;
    output.position.y = output.position.y / frustumTop;
    output.position.z = (output.position.z - near) / (far - near);
    output.position.w = output.position.w;
	/*
	// 行列で計算する場合
	// プロジェクション変換行列はCPU側で計算して定数バッファーに設定するのが定石となる。
	// このため定数ではないoutput.position.zは行列内に含むことはできない。
	const float4x4 Projection = {
		aspect / tan(fovY / 2),              0.0f,                 0.0f, 0.0f,
		                  0.0f, 1 / tan(fovY / 2),                 0.0f, 0.0f,
		                  0.0f,              0.0f,     1 / (far - near), 0.0f,
		                  0.0f,              0.0f, -near / (far - near), 1.0f,
	};
	// プロジェクション変換とは別にoutput.position.zの除算を追加する
	output.position.x /= output.position.z;
	output.position.y /= output.position.z;
	output.position = mul(output.position, Projection);
	*/
    
    output.color = input.color;
    return output;
}