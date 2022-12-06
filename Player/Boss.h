#pragma once
#include"Player.h"
#include"../staging/Easing.h"

class Boss
	:public Enemy
{
public:
	Boss();
	~Boss();

	/// <summary>
	/// ������
	/// </summary>
	void bossInit();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̃|�C���^�[</param>
	void bossUpdate(Player* player);

	/// <summary>
	/// �{�X�̃Z�b�e�B���O
	/// </summary>
	/// <param name="pos">���W</param>
	void bossSet(XMFLOAT3 pos);

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void bossReSet();

	/// <summary>
	/// �{�X�� H P �̃Z�b�e�B���O
	/// </summary>
	/// <param name="hp">�̗�</param>
	void setHitPoint(int hp)
	{
		HP = hp;
		resetHitPoint = hp;
		return;
	}

	/// <summary>
	/// �{�X�̓o�ꉉ�o
	/// </summary>
	/// <param name="player">�v���C���[�̃|�C���^�[</param>
	void bossArrival(Player* player);

	//�o�ꉉ�o�t���O�̎擾
	bool getIsAppear() { return isAppear; }

private:
	int resetHitPoint = 3;
	int arrivalTime = 0;
	//�o�ꉉ�o�t���O
	int isAppear = false;
	easingManager bossRotEase;
	XMFLOAT3 bossbaseScale;
	XMFLOAT3 bossScale;
};