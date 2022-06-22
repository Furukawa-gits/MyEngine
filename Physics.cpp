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

void Ball::Update(float friction)
{
	if (IsMove)
	{
		//�Ε�����
		if (IsThrow)
		{
			Throw();
			Time += (1.0f / 60.0f);
			return;
		}

		//�X���C�h
		if (IsSlide)
		{
			Slide(friction);
			Time += (1.0f / 60.0f);
			return;
		}
	}
}

void Ball::Throw()
{
	Pos.x += Accel.x;
	Pos.y += Accel.y;
	Pos.z += Accel.z;

	//�d�͉����x
	Accel.y += Gravitationalacceleration * Time;

	//�e�����̋�C��R�v�Z
	if (Accel.x > Airresistance)
	{
		Accel.x -= Airresistance;
	}
	else
	{
		Accel.x = 0;
	}

	if (Accel.y > Airresistance)
	{
		Accel.y -= Airresistance;
	}

	if (Accel.z > Airresistance)
	{
		Accel.z -= Airresistance;
	}
	else
	{
		Accel.z = 0;
	}
}

void Ball::Slide(float friction)
{
	Pos.x += Accel.x;
	Pos.z += Accel.z;

	//�e�����̖��C�v�Z
	if (Accel.x > friction)
	{
		Accel.x -= friction;
	}
	else
	{
		Accel.x = 0;
	}

	if (Accel.z > friction)
	{
		Accel.z -= friction;
	}
	else
	{
		Accel.z = 0;
	}
}
