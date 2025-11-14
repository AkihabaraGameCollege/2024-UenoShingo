#pragma once

namespace GameLibrary
{
	// ŠÔ‚ğŒv‘ª‚µ‚Ü‚·B
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
