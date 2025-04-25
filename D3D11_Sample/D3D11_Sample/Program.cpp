#include <Windows.h>

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
	// メッセージボックスを表示
	MessageBoxW(NULL,L"ここにメッセージが表示されます。",L"タイトル", MB_ABORTRETRYIGNORE);

	return 0;
}