#pragma once
#include<Windows.h>



const int windowWidth = 1280;
const int windowHight = 720;

class Window_Generate
{
public:
	//変数

	WNDCLASSEX w{};//ウィンドウクラス

	RECT wrc = { 0,0,windowWidth,windowHight };//ウィンドウサイズ

	HWND hwnd{};

	MSG msg{};//メッセージ


	//関数
	//Window_Generate();

	static LRESULT Win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	void Win_Init();//クラス初期化
	void Win_Create();//ウィンドウ生成＆表示
	void Win_Deleate();//ウィンドウクラス登録解除


};