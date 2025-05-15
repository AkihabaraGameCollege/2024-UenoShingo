//=============================================================================
// Program.cpp
//
// アプリケーションのエントリーポイントが含まれます。
//=============================================================================
#include "Game.h"

// エントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	Game game(L"タイトル", 1280, 720);
	return game.Run();
}
