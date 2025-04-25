#include <Windows.h>

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
	// MB_OKCANCELを指定すると『OK』ボタンと『CANCEL』ボタンを表示
	if (MessageBox(NULL, L"ここにメッセージが表示されます。", L"質問",
		MB_OKCANCEL) == IDOK) {
		MessageBox(NULL, L"OKされました。", L"結果", MB_OK);
	}
	else {
		MessageBox(NULL, L"キャンセルされました。", L"結果", MB_OK);
	}

	return 0;
}