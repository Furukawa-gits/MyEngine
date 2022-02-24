#include"Fps_Manager.h"

HRESULT FpsManager::init()
{
	if (QueryPerformanceFrequency(&timeFreq) == FALSE)
	{
		return(E_FAIL);
	}

	QueryPerformanceCounter(&timeStart);
}

void FpsManager::Run()
{
	QueryPerformanceCounter(&timeEnd);

	frametime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frametime < MIN_FREAM_TIME)
	{
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frametime) * 1000);

		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);

		return;
	}

	if (frametime > 0.0)
	{
		fps = (fps * 0.99f) + (0.01f / frametime);

#ifdef _DEBUG

#ifdef UNICODE
		std::wstringstream stream;

#else
		std::stringstream stream;

#endif // DEBUG
		stream << fps << " FPS" << std::endl;

		OutputDebugString(stream.str().c_str());

#endif // DEBUG

	}

	timeStart = timeEnd;
}
