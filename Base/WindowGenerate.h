#pragma once
#include<Windows.h>



const int windowWidth = 1280;
const int windowHight = 720;

class Window_Generate
{
public:
	//�ϐ�

	WNDCLASSEX w{};//�E�B���h�E�N���X

	RECT wrc = { 0,0,windowWidth,windowHight };//�E�B���h�E�T�C�Y

	HWND hwnd{};

	MSG msg{};//���b�Z�[�W


	//�֐�
	//Window_Generate();

	static LRESULT Win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	void Win_Init();//�N���X������
	void Win_Create();//�E�B���h�E�������\��
	void Win_Deleate();//�E�B���h�E�N���X�o�^����


};