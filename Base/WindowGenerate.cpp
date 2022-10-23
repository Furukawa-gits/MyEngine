#include"WindowGenerate.h"

//Window_Generate::Window_Generate()
//{
//	
//}

LRESULT Window_Generate::Win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージ分岐
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		PostQuitMessage(0);//OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理を行う

}

//クラス初期化
void Window_Generate::Win_Init()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)Win_proc;
	w.lpszClassName = L"cg2_game_project";
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
}

//ウィンドウ生成＆表示
void Window_Generate::Win_Create()
{
	//ウィンドウ生成
	hwnd = CreateWindow(
		w.lpszClassName,
		L"DragShoot",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	//ウィンドウサイズ
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

//ウィンドウクラス登録解除
void Window_Generate::Win_Deleate()
{
	UnregisterClass(w.lpszClassName, w.hInstance);
}


