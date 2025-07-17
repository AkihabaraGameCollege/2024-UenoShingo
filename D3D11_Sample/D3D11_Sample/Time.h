#pragma once
#pragma once

#include <Windows.h>

#pragma comment (lib, "Winmm.lib")

// ŠÔ‚ğŒv‘ª‚µ‚Ü‚·B
class Time
{
public:
	static void Initialize();
	static void Shutdown();
	static void Update();

	static float GetDeltaTime();
};