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
		//斜方投射
		if (IsThrow)
		{
			Throw();
			Time += (1.0f / 60.0f);
			return;
		}

		//スライド
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

	//重力加速度
	Accel.y += Gravitationalacceleration * Time;

	//各方向の空気抵抗計算
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

	//各方向の摩擦計算
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
