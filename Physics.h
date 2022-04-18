#pragma once
#include<DirectXMath.h>

using namespace DirectX;

//�����v�Z
class Physics
{
public:
	/// <summary>
	/// ���R����
	/// </summary>
	/// <param name="y">���W</param>
	/// <param name="t">����(�b)</param>
	float FreeFall(float y, float t);

	const float Gravitationalacceleration = 9.8f;

	float Time;
};

//�{�[��
class Ball : public Physics
{
public:
	//�ʒu
	XMFLOAT3 Pos = { 0,0,0 };
	//�����x
	XMFLOAT3 StartSpeed = { 0,0,0 };
	//�����x
	XMFLOAT3 Accel = { 0,0,0 };

	bool IsMove = false;

	/// <summary>
	/// �{�[���ɗ͂�������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="startspeed">����</param>
	void Set(XMFLOAT3 pos, XMFLOAT3 startspeed);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
};
