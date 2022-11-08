#include "Shake.h"

Shake::Shake()
{
}

Shake::~Shake()
{
}

void Shake::set(int shakewide, int totalTime)
{
	wide = shakewide;
	totalFrame = totalTime;
	srand(time(NULL));
}

int Shake::shaking()
{
	if (!isActive)
	{
		return 0;
	}

	totalFrame--;

	shakeReturn = (rand() % (wide * 2)) - wide;

	if (totalFrame <= 0)
	{
		isActive = false;
		shakeReturn = 0;
	}

	return shakeReturn;
}
