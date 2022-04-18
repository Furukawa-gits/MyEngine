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

	const float Gravitationalacceleration = 9.8f;

	float Time;
};

//ボール
class Ball : public Physics
{
public:
	//位置
	XMFLOAT3 Pos = { 0,0,0 };
	//初速度
	XMFLOAT3 StartSpeed = { 0,0,0 };
	//加速度
	XMFLOAT3 Accel = { 0,0,0 };

	bool IsMove = false;

	/// <summary>
	/// ボールに力を加える
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="startspeed">初速</param>
	void Set(XMFLOAT3 pos, XMFLOAT3 startspeed);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
};
