#include "Time.h"
#include <timeapi.h>

namespace
{
	DWORD s_LastTime = 0;
	float s_DeltaTime = 0;
}

/// <summary>
/// 時間計測を開始します。
/// </summary>
void Time::Initialize()
{
	timeBeginPeriod(0);
}

/// <summary>
/// 時間計測を終了します。
/// </summary>
void Time::Shutdown()
{
	timeEndPeriod(0);
}

/// <summary>
/// アプリケーションから、この関数を毎フレーム呼び出して現在時間を更新します。
/// </summary>
void Time::Update()
{
	const auto currentTime = timeGetTime();
	s_DeltaTime = (currentTime - s_LastTime) * 0.001f;
	s_LastTime = currentTime;
}

/// <summary>
/// 前回のフレームからの差分時間を取得します。
/// </summary>
/// <returns>差分時間(秒)</returns>
float Time::GetDeltaTime()
{
	return s_DeltaTime;
}
