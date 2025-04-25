#include <Windows.h>

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    HWND hWnd = NULL;

    hWnd = CreateWindowExW(
        0,  L"Static",L"タイトル",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,640, 480,
        NULL,NULL,hInstance,NULL);
    if (hWnd == NULL) {
        MessageBox(NULL, L"ウィンドウの作成に失敗しました。", L"エラー", MB_OK);
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MessageBox(hWnd, L"ウィンドウが表示されましたか？", L"確認", MB_OK);

    return 0;
}
