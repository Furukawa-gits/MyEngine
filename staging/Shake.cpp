#include "Shake.h"

Shake::Shake()
{
}

Shake::~Shake()
{
}

void Shake::set(shakeType type, float shakewide, int time)
{
	thisType = type;
	wide = shakewide;
	totalFrame = time;
}

float Shake::shaking()
{
	if (!isActive)
	{
		return 0;
	}

	totalFrame--;

	shakeVertical();
	shakehorizontal();
	shakeFreely();

	if (totalFrame <= 0)
	{
		isActive = false;
		shakeReturn = 0;
	}

	return shakeReturn;
}

void Shake::shakeVertical()
{
}

void Shake::shakehorizontal()
{
}

void Shake::shakeFreely()
{
}
