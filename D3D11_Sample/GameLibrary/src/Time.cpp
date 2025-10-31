#include <GameLibrary/Time.h>
#include <GameLibrary/Utility.h>
#include <timeapi.h>

using namespace GameLibrary;

namespace
{
	constexpr DWORD s_MaxDeltaTime = 19;
	DWORD s_LastTime = 0;
	DWORD s_DeltaTime = 0;
}

/// <summary>
/// 時間計測を開始します。
/// </summary>
void Time::Initialize()
{
	timeBeginPeriod(1);
}

/// <summary>
/// 時間計測を終了します。
/// </summary>
void Time::Shutdown()
{
	timeEndPeriod(1);
}

/// <summary>
/// アプリケーションから、この関数を毎フレーム呼び出して現在時間を更新します。
/// </summary>
void Time::Update()
{
	DWORD currentTime;
	// 最初のフレーム
	if (s_LastTime == 0) {
		currentTime = s_MaxDeltaTime;
	}
	else {
		currentTime = timeGetTime();
	}

	s_DeltaTime = currentTime - s_LastTime;
	s_LastTime = currentTime;
}

/// <summary>
/// 前回のフレームからの差分時間を取得します。
/// </summary>
/// <returns>差分時間(秒)</returns>
float Time::GetDeltaTime()
{
	return s_DeltaTime * 0.001f;
}
