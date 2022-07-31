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

void Ball::setTarget(XMFLOAT3 pos, XMFLOAT3 speed)
{
	Pos = pos;

	StartSpeed = { speed.x,speed.y,speed.z };
	Accel = StartSpeed;

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
		}

		//スライド
		if (IsSlide)
		{
			Slide(friction);
			Time += (1.0f / 60.0f);
		}
	}

	//今持ってる運動エネルギーを求める
	E.x = 0.5f * M * powf(fabs(Accel.x), 2);
	E.y = 0.5f * M * powf(fabs(Accel.y), 2);
	E.z = 0.5f * M * powf(fabs(Accel.z), 2);
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
	//速度加算
	Pos.x += Accel.x;
	Pos.y += Accel.y;
	Pos.z += Accel.z;

	//速度の絶対値
	XMFLOAT3 accelAbs =
	{
		fabsf(Accel.x),
		fabsf(Accel.y),
		fabsf(Accel.z)
	};

	//各方向の摩擦計算
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
	//2つのボールの距離
	XMFLOAT3 dis =
	{
		fabsf(this->Pos.x - ball2.Pos.x),
		fabsf(this->Pos.y - ball2.Pos.y),
		fabsf(this->Pos.z - ball2.Pos.z)
	};

	//直線距離(上のベクトルの大きさ)
	float disVec = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	//計算する前の速度
	XMFLOAT3 oldAccel = this->Accel;
	XMFLOAT3 oldAccel2 = ball2.Accel;

	//衝突した時
	if (disVec <= Radius.x + ball2.Radius.x)
	{
		//相手のボールにエネルギーを与える
		ball2.E = this->E;

		//ベクトルの向きに応じて標的にも速度を与える
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

		//速度ベクトルを反転(正面衝突想定)
		Accel.x *= -1 * repulsion;
		Accel.y *= -1 * repulsion;
		Accel.z *= -1 * repulsion;
	}
}

void Ball::circularMotion2D(XMFLOAT3 center, float G)
{
	if (!IsMove)
	{
		return;
	}

	XMFLOAT3 dis =
	{
		center.x - Pos.x,
		center.y - Pos.y,
		center.z - Pos.z
	};

	float disLength = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	XMFLOAT3 disNormalize =
	{
		dis.x / disLength,
		dis.y / disLength,
		dis.z / disLength
	};

	Accel.x += (disNormalize.x * G);
	Accel.y += (disNormalize.y * G);
	Accel.z += 0.0f;

	//速度加算
	Pos.x += Accel.x;
	Pos.y += Accel.y;
	Pos.z = 0.0f;
}

dxinput* strings::input = nullptr;

void strings::setInput(dxinput* in)
{
	input = in;
}

strings::strings()
{
	
}

void strings::update()
{
	hp[idx_hist % numHist].x = pos.x;
	hp[idx_hist % numHist].y = pos.y;
	idx_hist++;

	if (grabbed)
	{
		pos = { input->mouse_position.x,input->mouse_position.y };
		if (!input->Mouse_LeftPush())
			grabbed = 0;
	}
	else
	{
		XMFLOAT2 dis = { input->mouse_position.x - pos.x,input->mouse_position.y - pos.y };
		double _dst = sqrtf(powf(dis.x, 2) + powf(dis.y, 2));
		if (input->Mouse_LeftPush())
		{
			if (_dst < r)
				grabbed = 1;
		}

	}

	// 0:存在してない　1:存在して活動中　2:存在してて活動してない
	if (stat == 2)
		return;

	{
		// acc:加速度
		acc.x = 0.0f;
		acc.y = g;	//重力加速度（値は適当で良い）

		if (link0)	//親OBJがある?
		{
			// 自分の1つ上のobjへのベクトル
			XMFLOAT2 _dist = { link0->pos.x - pos.x,link0->pos.y - pos.y };
			// ↑のベクトルの大きさ
			double	_len = sqrtf(powf(_dist.x, 2) + powf(_dist.y, 2));

			// 親objとの距離が標準距離（LENGTH_LINK）より大きければ
			// 加速度を加算
			if (_len > lengthLink)
			{
				// ベクトル(_dist)＊標準距離からの増分
				_dist = { _dist.x * ((float)_len - lengthLink),_dist.y * ((float)_len - lengthLink)};

				_dist = { _dist.x / lengthLink,_dist.y / lengthLink };

				// K_HOOK:ばね定数（伸びにくさ）
				// 質量(m)が大きいほど_distを掛けた加速度(acc)の増分は減る
				XMFLOAT2 add_acc = { _dist.x * hookK / (float)m,_dist.y * hookK / (float)m };
				acc.x += add_acc.x;
				acc.y += add_acc.y;
			}
		}
		if (link1)	//子OBJがある?
		{
			// 自分の１つ下のobjへのベクトル
			XMFLOAT2 _dist = { link1->pos.x - pos.x,link1->pos.y - pos.y };
			// ↑のベクトルの大きさ
			double	_len = sqrtf(powf(_dist.x, 2) + powf(_dist.y, 2));

			if (_len > lengthLink)
			{
				_dist = { _dist.x * ((float)_len - lengthLink) / lengthLink,_dist.y * ((float)_len - lengthLink) / lengthLink};
				acc.x += _dist.x * hookK / m;
				acc.y += _dist.y * hookK / m;
			}
		}

		vel.x += acc.x;			//速度+=加速度
		vel.y += acc.y;			//速度+=加速度

		vel.x -= vel.x * resK;	//粘性抵抗
		vel.y -= vel.y * resK;	//粘性抵抗
								// (物体が近傍の流体を引きずることによって受ける反作用)

		pos.x += vel.x;			//位置+=速度
		pos.y += vel.y;			//位置+=速度
	}

	ball.position = { pos.x,pos.y,0 };
	ball.SpriteUpdate();
	if (link0)
	{
		line.SetLineSprite({ pos.x,pos.y,0 }, { link0->pos.x,link0->pos.y,0.0f });
	}
	else
	{
		line.SetLineSprite({ pos.x,pos.y,0 }, { pos.x,pos.y,0 });
	}
}

void strings::disp(ID3D12GraphicsCommandList* cmdList)
{
	ball.DrawSprite(cmdList);
	line.DrawSprite(cmdList);
}

void strings::init()
{
	pos = { 0,0 };
	this->r = R;
	dist = 0;
	vel = { 0,0 };
	acc = { 0,0 };
	m = 1.0f;
	stat = 0;
	idx_hist = 0;
	for (int i = 0; i < numHist; i++)
	{
		hp[i] = { 0,0 };
	}
	link0 = NULL;
	link1 = NULL;
	grabbed = 0;
	size = { 10,10 };
	idx = 1;

	ball.anchorpoint = { 0.5f,0.5f };
	ball.size = { (float)r,(float)r };
	ball.GenerateSprite("Ball.png");
	line.GenerateSprite("white1x1.png");
}
