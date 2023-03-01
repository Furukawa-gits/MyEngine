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
	/// ���f���ǂݍ���
	/// </summary>
	static void staticInitBoss();

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
	/// �X�v���C�g�̍X�V
	/// </summary>
	void bossSpriteUpdata();

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

	/// <summary>
	/// ��������
	/// </summary>
	void bossAriveMove();

	/// <summary>
	/// ���ď���
	/// </summary>
	void bossDeathMove();

	/// <summary>
	/// �v���C���[�Ǐ]
	/// </summary>
	void bossChase();

	/// <summary>
	/// �ˌ�
	/// </summary>
	void bossShot();

	/// <summary>
	/// �ǔ����ˌ�
	/// </summary>
	void bossHoming();

	/// <summary>
	/// ����
	/// </summary>
	void bossRampage();

	//�o�ꉉ�o�t���O�̎擾
	bool getIsAppear() { return isAppear; }

	//���f��
	static std::unique_ptr<Model> bossModel;
private:

	int resetHitPoint = 3;
	int arrivalTime = 0;
	easingManager bossRotEase;
	XMFLOAT3 bossbaseScale;
	XMFLOAT3 bossScale;

	Player* playerPointer = nullptr;

	//�{�X�o�����o�p�̃J����
	Camera* bossCamera;
};