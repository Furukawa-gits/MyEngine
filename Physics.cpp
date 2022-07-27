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
		}

		//�X���C�h
		if (IsSlide)
		{
			Slide(friction);
			Time += (1.0f / 60.0f);
		}
	}

	//�������Ă�^���G�l���M�[�����߂�
	E.x = 0.5f * M * powf(fabs(Accel.x), 2);
	E.y = 0.5f * M * powf(fabs(Accel.y), 2);
	E.z = 0.5f * M * powf(fabs(Accel.z), 2);
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
	//���x���Z
	Pos.x += Accel.x;
	Pos.y += Accel.y;
	Pos.z += Accel.z;

	//���x�̐�Βl
	XMFLOAT3 accelAbs =
	{
		fabsf(Accel.x),
		fabsf(Accel.y),
		fabsf(Accel.z)
	};

	//�e�����̖��C�v�Z
	if (accelAbs.x > friction)
	{
		if (Accel.x > 0)
		{
			Accel.x -= friction;
		}
		else if (Accel.x < 0)
		{
			Accel.x += friction;
		}
	}
	else
	{
		Accel.x = 0;
	}

	if (accelAbs.y > friction)
	{
		if (Accel.y > 0)
		{
			Accel.y -= friction;
		}
		else if (Accel.y < 0)
		{
			Accel.y += friction;
		}
	}
	else
	{
		Accel.y = 0;
	}

	if (accelAbs.z > friction)
	{
		if (Accel.z > 0)
		{
			Accel.z -= friction;
		}
		else if (Accel.z < 0)
		{
			Accel.z += friction;
		}
	}
	else
	{
		Accel.z = 0;
	}
}

void Ball::hitBallSlide(Ball& ball2)
{
	//2�̃{�[���̋���
	XMFLOAT3 dis =
	{
		fabsf(this->Pos.x - ball2.Pos.x),
		fabsf(this->Pos.y - ball2.Pos.y),
		fabsf(this->Pos.z - ball2.Pos.z)
	};

	//��������(��̃x�N�g���̑傫��)
	float disVec = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	//�v�Z����O�̑��x
	XMFLOAT3 oldAccel = this->Accel;
	XMFLOAT3 oldAccel2 = ball2.Accel;

	//�Փ˂�����
	if (disVec <= Radius.x + ball2.Radius.x)
	{
		//����̃{�[���ɃG�l���M�[��^����
		ball2.E = this->E;

		//�x�N�g���̌����ɉ����ĕW�I�ɂ����x��^����
		if (oldAccel.x > 0)
		{
			ball2.Accel.x = sqrtf(ball2.E.x / (0.5 * ball2.M));
			ball2.IsMove = true;
		}
		else if (oldAccel.x < 0)
		{
			ball2.Accel.x = -(sqrtf(ball2.E.x / (0.5 * ball2.M)));
			ball2.IsMove = true;
		}

		if (oldAccel.y > 0)
		{
			ball2.Accel.y = sqrtf(ball2.E.y / (0.5 * ball2.M));
			ball2.IsMove = true;
		}
		else if (oldAccel.y < 0)
		{
			ball2.Accel.y = -(sqrtf(ball2.E.y / (0.5 * ball2.M)));
			ball2.IsMove = true;
		}

		if (oldAccel.z > 0)
		{
			ball2.Accel.z = sqrtf(ball2.E.z / (0.5 * ball2.M));
			ball2.IsMove = true;
		}
		else if (oldAccel.z < 0)
		{
			ball2.Accel.z = -(sqrtf(ball2.E.z / (0.5 * ball2.M)));
			ball2.IsMove = true;
		}

		//���x�x�N�g���𔽓](���ʏՓˑz��)
		Accel.x *= -1 * repulsion;
		Accel.y *= -1 * repulsion;
		Accel.z *= -1 * repulsion;
	}
}

void Ball::circularMotion2D(XMFLOAT3 center, float G)
{

}
