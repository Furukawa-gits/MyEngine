#pragma once
#include<DirectXMath.h>
#include"2D/SpriteSingleunit.h"
#include"Input/dxInput.h"

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
	//�����W��
	float repulsion = 1.0f;

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
	/// �{�[���ɗ͂�������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="target">���W�����Ƃ̑��x</param>
	/// <param name="startspeed">����</param>
	void setTarget(XMFLOAT3 pos, XMFLOAT3 speed);

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

	/// <summary>
	/// �w�肵�����W�ւ̌��S�͉^��
	/// </summary>
	/// <param name="center">���S�_</param>
	/// <param name="radius">�d��</param>
	void circularMotion2D(XMFLOAT3 center, float G);
};

const float R = 30;
const float lengthLink = 50;
const float hookK = 0.01f;
const float resK = 0.1f;
const float g = 0.2f;
const int numHist = 10;

class strings
{
public:
	static dxinput* input;

	int stat;

	XMFLOAT2 pos;
	XMFLOAT2 size = { 10,10 };
	XMFLOAT2 vel;
	XMFLOAT2 acc;
	double m;
	double r;
	double dist;
	XMFLOAT2 hp[numHist];
	int idx_hist;
	int idx;
	strings* link0;
	strings* link1;
	int grabbed;

	static void setInput(dxinput* in);

	SingleSprite ball;
	SingleSprite line;

	strings();
	void init();
	void update();
	void disp(ID3D12GraphicsCommandList* cmdList);
};
