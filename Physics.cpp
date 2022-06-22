#include"Physics.h"

float Physics::FreeFall(float y, float t)
{
	return y + t * Gravitationalacceleration;
}

void Ball::Set(XMFLOAT3 pos, XMFLOAT3 startspeed)
{
	Pos = pos;
	StartSpeed = startspeed;
	Accel = startspeed;

	Time = 0;

	IsMove = true;
}

void Ball::Update()
{
	if (IsMove)
	{
		Pos.x += Accel.x;
		Pos.y += Accel.y;
		Pos.z += Accel.z;

		//d—Í‰Á‘¬“x
		Accel.y += Gravitationalacceleration * Time;

		if (Accel.x > Airresistance)
		{
			Accel.x -= Airresistance;
		}

		if (Accel.y > Airresistance)
		{
			Accel.y -= Airresistance;
		}

		if (Accel.z > Airresistance)
		{
			Accel.z -= Airresistance;
		}

		Time += (1.0f / 60.0f);
	}
}
