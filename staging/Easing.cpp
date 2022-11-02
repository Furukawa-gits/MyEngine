#include "Easing.h"

easingManager::easingManager()
{
}

easingManager::~easingManager()
{
}

void easingManager::set(easingPattern pattaern, easingLevel level, int totaltime, float start, float end)
{
	thisPattern = pattaern;
	thisLevel = level;
	totalFrame = totaltime;
	this->start = start;
	this->end = end;
	frame = 0.0f;
	rate = 0.0f;
	isActive = true;
}

float easingManager::easing()
{
	if (!isActive)
	{
		return 0.0f;
	}



	return easingNum;
}

void easingManager::easeIn()
{
	if (thisPattern != easingPattern::easeIn)
	{
		return;
	}
}

void easingManager::easeOut()
{
	if (thisPattern != easingPattern::easeOut)
	{
		return;
	}
}

void easingManager::easeInOut()
{
	if (thisPattern != easingPattern::easeInOut)
	{
		return;
	}
}

void easingManager::inQuadratic()
{
}

void easingManager::inCubic()
{
}

void easingManager::inQuartic()
{
}

void easingManager::inQuintic()
{
}

void easingManager::inSinusoidal()
{
}

void easingManager::inExponential()
{
}

void easingManager::inCircular()
{
}

