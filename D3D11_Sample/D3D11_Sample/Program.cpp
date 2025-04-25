#include <Windows.h>

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
	CHAR str[] = "ABC‚ ‚¢‚¤\n";
	OutputDebugStringA(str);

	WCHAR strW[] = L"ABC‚ ‚¢‚¤\n";
	OutputDebugStringW(strW);
	OutputDebugString(strW);

	return 0;
}