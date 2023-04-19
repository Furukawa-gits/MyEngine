#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include"../3D/ObjectCommon.h"

class dxinput
{
private:
	//�L�[�{�[�h
	HRESULT result;
	IDirectInput8* dinput = nullptr;
	IDirectInputDevice8* devkeyboard = nullptr;
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	//�}�E�X
	IDirectInput8* dinput_mouse = nullptr;
	IDirectInputDevice8* devmouse = nullptr;
	DIMOUSESTATE mouse;
	DIMOUSESTATE oldmouse;

public:

	//�}�E�X���W
	POINT mousePoint;
	POINT oldMousePoint;

	XMFLOAT3 mousePosition = { 0,0,0 };
	XMFLOAT3 oldMousePosition = { 0,0,0 };

	XMFLOAT2 mouseMoveVecrocity = { 0,0 };

	//�֐��R
	dxinput();

	//������
	void init(WNDCLASSEX w, HWND hwnd);

	//�X�V
	void Update(HWND hwnd);

	//�������u��
	bool Triger(BYTE keyCode);

	//�����Ă����
	bool push(BYTE keyCode);

	//�N���b�N�����u��
	bool Mouse_LeftTriger();

	//�����Ă����
	bool Mouse_LeftPush();

	//�������u��
	bool Mouse_LeftRelease();
};
