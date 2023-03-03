#pragma once
#include "Boss.h"

class uniteParts
	:public Enemy
{
public:
	void initParts(int ID);

	void setStaticData(XMFLOAT3* motherposition);

	void updataParts();

	void setParts(XMFLOAT3 position, float theta, float phi);

private:

	static float defaultPosition;

	//�p�[�c���Ƃ̌ŗL�ԍ�(1�`�p�[�c�̌�)
	int partsID = 0;

	//�p�[�c���Ƃ�HP
	std::list<std::unique_ptr<SingleSprite>> partsHitPoint;

	//�p�[�c�̍��W
	XMFLOAT3 partsPosition = { 0,0,0 };

	float angleTheta;
	float anglePhi;
};

class uniteBoss
	:public Enemy
{
public:
	uniteBoss();
	~uniteBoss();

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void uniteBossStaticInit();

	/// <summary>
	/// ������
	/// </summary>
	void uniteBossInit();

	/// <summary>
	/// �X�V
	/// </summary>
	void uniteBossUpdata();

	/// <summary>
	/// �X�v���C�g�X�V
	/// </summary>
	void uniteBossSpriteUpdata();

	/// <summary>
	/// �Z�b�g
	/// </summary>
	void uniteBossSet();

	/// <summary>
	/// �o�ꉉ�o
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void uniteBossArrival(Player* player);

	/// <summary>
	/// ����������
	/// </summary>
	void uniteBossAriveMove();

	/// <summary>
	/// uniteboss��p���j���o
	/// </summary>
	void uniteBossDeathMove();

	/// <summary>
	/// �s���p�^�[�����I
	/// </summary>
	void selectMovePattern();

	/// <summary>
	/// �v���C���[�Ǐ]
	/// </summary>
	void uniteBossChargeAttack();

	/// <summary>
	/// �ˌ�(����)
	/// </summary>
	void uniteBossRampage();

	/// <summary>
	/// �ˌ�(�v���C���[�_��)
	/// </summary>
	void uniteBossShotPlayerTarget();

	/// <summary>
	/// �`��(3d)
	/// </summary>
	/// <param name="directx"></param>
	void uniteBossDraw3d(directX* directx);

	/// <summary>
	/// �`��(2d)
	/// </summary>
	/// <param name="directx"></param>
	void uniteBossDraw2d(directX* directx);

private:

	//���f��
	static std::unique_ptr<Model> uniteBossModel;

	//�p�[�c�̐�
	static const int unitNum = 6;

	//�v���C���[�̃A�h���X
	static Player* player;

	//�{�̂���p�[�c�܂ł̋���
	static const float partsRadius;

	//�p�[�c����
	std::list<std::unique_ptr<uniteParts>> partsList;

	//�p�[�c�̋��ʍ��W��ݒ肷��Ƃ��̃C���f�b�N�X
	XMFLOAT2 defaultPartsAngle[6] =
	{
		{0,0},
		{90,0},
		{180,0},
		{270,0},
		{0,90},
		{0,180}
	};

	//HP�Q�[�W
	std::list<std::unique_ptr<SingleSprite>> motherHitPoint;

	//�U���p�^�[���ꗗ
	enum class uniteBossPattern
	{
		shotPlayerTarget = 0,
		chargeAttack = 1,
		rampageBullet = 2
	};

	//���݂̍U���p�^�[��
	uniteBossPattern nowPattern;

	/// <summary>
	/// �U���ڕW
	/// <para>�ːi�F�˂����ޏꏊ</para>
	/// <para>�ˌ�(���@�_��)�F�v���C���[�̈ʒu</para>
	/// <para>���ˁF�x�N�g�� [ �{��->�p�[�c ] �̉�������</para>
	/// </summary>
	XMFLOAT3 targetPos;

	//�S�Ẵp�[�c����ꂽ���ǂ���
	bool isAllPartsBreak = false;

	//���o�p�ϐ�
	XMFLOAT3 arrivalStartPos;
	XMFLOAT3 arrivalEndPos;

	//�{�X���o�p�̃J����
	Camera* bossCamera;
};