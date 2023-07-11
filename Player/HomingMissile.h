#pragma once
#include"PrimitiveBullet.h"
#include"Enemy.h"

#include<memory>

//�z�[�~���O�~�T�C��
class Missile :
	public PrimitiveBullet
{
public:

	Enemy* enemyPointer = nullptr;

	bool isTargetSet = false;

	XMFLOAT3 bulletVecIndex[8] = {
		{  30.0f,  0.0f,0.0f },
		{ -30.0f,  0.0f,0.0f },
		{   0.0f, 30.0f,0.0f },
		{   0.0f,-30.0f,0.0f },
		{  30.0f, 30.0f,0.0f },
		{  30.0f,-30.0f,0.0f },
		{ -30.0f, 30.0f,0.0f },
		{ -30.0f,-30.0f,0.0f }
	};

	Missile();
	~Missile();

	void init(XMFLOAT4 motherColor, XMFLOAT4 childColor) override;
	void updata() override;
	void draw(directX* directx) override;

	void setPenemy(Enemy* enemy);
	void start(XMFLOAT3 start_pos);
	/// <summary>
	/// �ʏ�e�ƈႢ�A�����������̂Ńp�[�e�B�N���̍X�V�͕�����
	/// </summary>
	void particleUpdata();
};