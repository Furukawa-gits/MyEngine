#pragma once
#include<DirectXMath.h>

using namespace DirectX;

//物理計算
class Physics
{
public:
	/// <summary>
	/// 自由落下
	/// </summary>
	/// <param name="y">座標</param>
	/// <param name="t">時間(秒)</param>
	float FreeFall(float y, float t);

	//重力加速度
	const float Gravitationalacceleration = -9.8f;

	//空気抵抗
	float Airresistance = 0.8f;

	//経過時間
	float Time;
};

//ボール
class Ball : public Physics
{
public:
	//位置
	XMFLOAT3 Pos = { 0,0,0 };
	//半径
	XMFLOAT3 Radius = { 10,10,10 };
	//初速度
	XMFLOAT3 StartSpeed = { 0,0,0 };
	//加速度
	XMFLOAT3 Accel = { 0,0,0 };
	//質量
	float M = 1.0f;
	//エネルギー
	XMFLOAT3 E = {};

	bool IsMove = false;

	bool IsThrow = false;

	bool IsSlide = false;

	/// <summary>
	/// ボールに力を加える
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="startspeed">初速</param>
	void Set(XMFLOAT3 pos, XMFLOAT3 startspeed);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(float friction);

	/// <summary>
	/// 投射
	/// </summary>
	void Throw();

	/// <summary>
	/// スライド
	/// </summary>
	/// <param name="friction">摩擦係数</param>
	void Slide(float friction);

	/// <summary>
	/// 衝突
	/// </summary>
	/// <param name="ball2">当てる相手</param>
	void hitBallSlide(Ball& ball2);
};
