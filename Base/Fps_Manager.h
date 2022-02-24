#pragma once
#include<Windows.h>
#include<tchar.h>
#include<sstream>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")



class FpsManager
{
public:
	const float MIN_FREAM_TIME = 1.0f / 60;
	float frametime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;

	float fps = 0;

	//èâä˙âª
	HRESULT init();

	//FPSí≤êÆ
	void Run();

};
