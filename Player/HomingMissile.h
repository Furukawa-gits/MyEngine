#pragma once
#include"PrimitiveBullet.h"
#include"Enemy.h"
#include"../Math/SplineCurve.h"

#include<memory>

//�z�[�~���O�~�T�C��
class Missile :
	public PrimitiveBullet
{
private:

	/// <summary>
	/// �e�̖{�̂ƂȂ�p�[�e�B�N��
	/// <para>�e�̐����Ɉˑ�����̂ł����͎��Ԍo�߂ŏ����Ȃ�</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	//�p�[�e�B�N�������ׂ̈̃J�E���g
	int particleCount = 0;

	//�G�ɓ��B����܂ł̃t���[����
	static const int toEnemyMaxFrame = 30;

	int aliveFrame = 0;

	//�X�v���C���Ȑ�
	SplineCurve missileCurve;

	std::vector<RKDVector3> missilePositionts;

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
	void particleUpdata();
};