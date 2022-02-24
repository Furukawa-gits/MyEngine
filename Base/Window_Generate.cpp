#include"Window_Generate.h"

//Window_Generate::Window_Generate()
//{
//	
//}

LRESULT Window_Generate::Win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W����
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ
		PostQuitMessage(0);//OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//�W���̏������s��

}

//�N���X������
void Window_Generate::Win_Init()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)Win_proc;
	w.lpszClassName = L"cg2_game_project";
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);
}

//�E�B���h�E�������\��
void Window_Generate::Win_Create()
{
	//�E�B���h�E����
	hwnd = CreateWindow(
		w.lpszClassName,
		L"DragShooter",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	//�E�B���h�E�T�C�Y
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

//�E�B���h�E�N���X�o�^����
void Window_Generate::Win_Deleate()
{
	UnregisterClass(w.lpszClassName, w.hInstance);
}


