//=============================================================================
// Game.h
//
// ウィンドウを作成してメッセージループを開始する機能が含まれます。
//=============================================================================
#pragma once

#include <Windows.h>
#include <string>

// アプリケーション全体を表します。
class Game
{
public:
	// コンストラクター
	Game(const std::wstring& windowTitle, int screenWidth, int screenHeight);

	int Run();

private:
	// ウィンドウのタイトル
	std::wstring windowTitle = L"タイトル";
	// ウィンドウの幅
	int screenWidth = 640;
	// ウィンドウの高さ
	int screenHeight = 480;
	// ウィンドウのハンドル
	HWND hWnd = NULL;

	bool InitWindow();
};
