#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include"../3D/ObjectCommon.h"

class dxinput
{
private:
	//キーボード
	HRESULT result;
	IDirectInput8* dinput = nullptr;
	IDirectInputDevice8* devkeyboard = nullptr;
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

	//マウス
	IDirectInput8* dinput_mouse = nullptr;
	IDirectInputDevice8* devmouse = nullptr;
	DIMOUSESTATE mouse;
	DIMOUSESTATE oldmouse;

public:

	//マウス座標
	POINT mousePoint;
	POINT oldMousePoint;

	XMFLOAT3 mousePosition = { 0,0,0 };
	XMFLOAT3 oldMousePosition = { 0,0,0 };

	XMFLOAT2 mouseMoveVecrocity = { 0,0 };

	//関数軍
	dxinput();

	//初期化
	void init(WNDCLASSEX w, HWND hwnd);

	//更新
	void Update(HWND hwnd);

	//押した瞬間
	bool Triger(BYTE keyCode);

	//押している間
	bool push(BYTE keyCode);

	//クリックした瞬間
	bool Mouse_LeftTriger();

	//押している間
	bool Mouse_LeftPush();

	//離した瞬間
	bool Mouse_LeftRelease();
};
