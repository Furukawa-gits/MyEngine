#pragma once
#include<Windows.h>



const int win_width = 1280;
const int win_hight = 720;

class Window_Generate
{
public:
	//�ϐ�

	WNDCLASSEX w{};//�E�B���h�E�N���X

	RECT wrc = { 0,0,win_width,win_hight };//�E�B���h�E�T�C�Y

	HWND hwnd{};

	MSG msg{};//���b�Z�[�W


	//�֐�
	//Window_Generate();

	static LRESULT Win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	void Win_Init();//�N���X������
	void Win_Create();//�E�B���h�E�������\��
	void Win_Deleate();//�E�B���h�E�N���X�o�^����


};