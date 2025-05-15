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
	Game game;
	game.WindowTitle = L"タイトル";
	game.ScreenWidth = 1280;
	game.ScreenHeight = 720;
	return Run(&game);
}
