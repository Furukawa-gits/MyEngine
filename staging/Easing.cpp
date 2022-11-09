#include "Easing.h"

easingManager::easingManager()
{
}

easingManager::~easingManager()
{
}

void easingManager::set(easingType pattaern, easingPattern type, int totaltime, float start, float end)
{
	thisType = pattaern;
	thisPattern = type;
	totalFrame = totaltime;
	this->start = start;
	this->lange = end - start;
	frame = 0.0f;
	rate = 0.0f;
	isActive = true;
}

float easingManager::easing()
{
	if (!isActive)
	{
		return start;
	}

	//フレーム加算
	frame++;

	//進行度の割合を算出
	rate = (float)frame / (float)totalFrame;

	//イーズイン
	easeIn();

	//イーズアウト
	easeOut();

	//イーズインアウト
	easeInOut();

	if (frame == totalFrame)
	{
		easingNum = start + lange;
		isActive = false;
	}

	return easingNum;
}

void easingManager::easeIn()
{
	if (thisType != easingType::easeIn)
	{
		return;
	}

	inQuadratic();
	inCubic();
	inQuartic();
	inQuintic();
	inSinusoidal();
	inExponential();
	inCircular();
}

void easingManager::easeOut()
{
	if (thisType != easingType::easeOut)
	{
		return;
	}

	outQuadratic();
	outCubic();
	outQuartic();
	outQuintic();
	outSinusoidal();
	outExponential();
	outCircular();
}

void easingManager::easeInOut()
{
	if (thisType != easingType::easeInOut)
	{
		return;
	}

	inOutQuadratic();
	inOutCubic();
	inOutQuartic();
	inOutQuintic();
	inOutSinusoidal();
	inOutExponential();
	inOutCircular();
}

#pragma region イーズイン
void easingManager::inQuadratic()
{
	if (thisPattern != easingPattern::Quadratic)
	{
		return;
	}

	easingNum = lange * rate * rate + start;
}

void easingManager::inCubic()
{
	if (thisPattern != easingPattern::Cubic)
	{
		return;
	}

	easingNum = lange * rate * rate * rate + start;
}

void easingManager::inQuartic()
{
	if (thisPattern != easingPattern::Quartic)
	{
		return;
	}

	easingNum = lange * rate * rate * rate * rate + start;
}

void easingManager::inQuintic()
{
	if (thisPattern != easingPattern::Quintic)
	{
		return;
	}

	easingNum = lange * rate * rate * rate * rate * rate + start;
}

void easingManager::inSinusoidal()
{
	if (thisPattern != easingPattern::Sinusoidal)
	{
		return;
	}

	easingNum = -lange * cosf(rate * (M_PI / 2.0f)) + start;
}

void easingManager::inExponential()
{
	if (thisPattern != easingPattern::Exponential)
	{
		return;
	}

	easingNum = lange * powf(2, (10 * (rate - 1))) + start;
}

void easingManager::inCircular()
{
	if (thisPattern != easingPattern::Circular)
	{
		return;
	}

	easingNum = -lange * (sqrtf(1 - rate * rate) - 1) + start;
}
#pragma endregion

#pragma region イーズアウト
void easingManager::outQuadratic()
{
	if (thisPattern != easingPattern::Quadratic)
	{
		return;
	}

	easingNum = -lange * rate * (rate - 2.0f) + start;
}

void easingManager::outCubic()
{
	if (thisPattern != easingPattern::Cubic)
	{
		return;
	}

	rate = rate - 1.0f;

	easingNum = lange * (rate * rate * rate + 1.0f) + start;
}

void easingManager::outQuartic()
{
	if (thisPattern != easingPattern::Quartic)
	{
		return;
	}

	rate = rate - 1.0f;

	easingNum = -lange * (rate * rate * rate * rate - 1.0f) + start;
}

void easingManager::outQuintic()
{
	if (thisPattern != easingPattern::Quintic)
	{
		return;
	}

	rate = rate - 1.0f;

	easingNum = lange * (rate * rate * rate * rate * rate + 1.0f) + start;
}

void easingManager::outSinusoidal()
{
	if (thisPattern != easingPattern::Sinusoidal)
	{
		return;
	}

	easingNum = rate * sinf(rate * (M_PI / 2.0f)) + start;
}

void easingManager::outExponential()
{
	if (thisPattern != easingPattern::Exponential)
	{
		return;
	}

	easingNum = rate * (-(powf(2.0f, -10.0f * rate)) + 1) + start;
}

void easingManager::outCircular()
{
	if (thisPattern != easingPattern::Circular)
	{
		return;
	}

	rate = rate - 1.0f;

	easingNum = lange * sqrtf(1 - rate * rate) + start;
}
#pragma endregion

#pragma region イーズインアウト
void easingManager::inOutQuadratic()
{
	if (thisPattern != easingPattern::Quadratic)
	{
		return;
	}

	rate *= 2.0f;

	if (rate < 1.0f)
	{
		easingNum = lange / 2.0f * rate * rate + start;
	}
	else
	{
		rate = rate - 1;

		easingNum = -lange / 2.0f * (rate * (rate - 2) - 1) + start;
	}
}

void easingManager::inOutCubic()
{
	if (thisPattern != easingPattern::Cubic)
	{
		return;
	}

	rate *= 2.0f;

	if (rate < 1.0f)
	{
		easingNum = lange / 2.0f * rate * rate * rate + start;
	}
	else
	{
		rate = rate - 2;

		easingNum = lange / 2.0f * (rate * rate * rate + 2) + start;
	}
}

void easingManager::inOutQuartic()
{
	if (thisPattern != easingPattern::Quartic)
	{
		return;
	}

	rate *= 2.0f;

	if (rate < 1.0f)
	{
		easingNum = lange / 2.0f * rate * rate * rate * rate + start;
	}
	else
	{
		rate = rate - 2;

		easingNum = -lange / 2.0f * (rate * rate * rate * rate - 2) + start;
	}
}

void easingManager::inOutQuintic()
{
	if (thisPattern != easingPattern::Quintic)
	{
		return;
	}

	rate *= 2.0f;

	if (rate < 1.0f)
	{
		easingNum = lange / 2.0f * rate * rate * rate * rate * rate + start;
	}
	else
	{
		rate = rate - 2;

		easingNum = lange / 2.0f * (rate * rate * rate * rate * rate + 2) + start;
	}
}

void easingManager::inOutSinusoidal()
{
	if (thisPattern != easingPattern::Sinusoidal)
	{
		return;
	}

	easingNum = -lange / 2.0f * (cosf(M_PI * rate) - 1) + start;
}

void easingManager::inOutExponential()
{
	if (thisPattern != easingPattern::Exponential)
	{
		return;
	}

	rate *= 2.0f;

	if (rate < 1.0f)
	{
		easingNum = lange / 2.0f * powf(2.0f, 10.0f * (rate - 1)) + start;
	}
	else
	{
		rate = rate - 1;

		easingNum = lange / 2.0f * (-(powf(2.0f, -10.0f * rate)) + 2) + start;
	}
}

void easingManager::inOutCircular()
{
	if (thisPattern != easingPattern::Circular)
	{
		return;
	}

	rate *= 2.0f;

	if (rate < 1.0f)
	{
		easingNum = -lange / 2.0f * sqrtf(1 - rate * rate) + start;
	}
	else
	{
		rate = rate - 2;

		easingNum = lange / 2.0f * (sqrtf(1 - rate * rate) + 1) + start;
	}
}
#pragma endregion
