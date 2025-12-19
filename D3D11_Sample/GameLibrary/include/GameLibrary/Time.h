#pragma once

namespace GameLibrary
{
	// 時間を計測します。
	class Time
	{
	public:
		static void Initialize();
		static void Shutdown() noexcept;
		static void Update() noexcept;

		static float GetTime() noexcept;
		static float GetDeltaTime() noexcept;
	};
}
