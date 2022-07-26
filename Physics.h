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

	//�d�͉����x
	const float Gravitationalacceleration = -9.8f;

	//��C��R
	float Airresistance = 0.8f;

	//�o�ߎ���
	float Time;
};

//�{�[��
class Ball : public Physics
{
public:
	//�ʒu
	XMFLOAT3 Pos = { 0,0,0 };
	//���a
	XMFLOAT3 Radius = { 10,10,10 };
	//�����x
	XMFLOAT3 StartSpeed = { 0,0,0 };
	//�����x
	XMFLOAT3 Accel = { 0,0,0 };
	//����
	float M = 1.0f;
	//�G�l���M�[
	XMFLOAT3 E = {};

	bool IsMove = false;

	bool IsThrow = false;

	bool IsSlide = false;

	/// <summary>
	/// �{�[���ɗ͂�������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="startspeed">����</param>
	void Set(XMFLOAT3 pos, XMFLOAT3 startspeed);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(float friction);

	/// <summary>
	/// ����
	/// </summary>
	void Throw();

	/// <summary>
	/// �X���C�h
	/// </summary>
	/// <param name="friction">���C�W��</param>
	void Slide(float friction);

	/// <summary>
	/// �Փ�
	/// </summary>
	/// <param name="ball2">���Ă鑊��</param>
	void hitBallSlide(Ball& ball2);
};
