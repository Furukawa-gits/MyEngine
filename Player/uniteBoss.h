#pragma once
#include "Boss.h"

class uniteParts
	:public Enemy
{
public:
	void partsInit(int ID);

	static void setStaticData(XMFLOAT3* motherposition);

	void partsUpdata();

	void partsArrival();

	void partsAriveMove();

	void partsDeathMove();

	void partsShotBullet(XMFLOAT3 targetposition);

	void partsSet(XMFLOAT3 position, float theta, float phi);

	void partsDraw3D(directX* directx);

	void partsDraw2D(directX* directx);

	//�p���x
	float angleSpeed = 0.01f;

private:

	const float pi = 3.1415f;
	const float radiannum = 180.0f;

	//���f��
	static std::unique_ptr<Model> partsModel;

	//�{�̂̍��W
	static XMFLOAT3* motherPosition;

	//�{�̂���p�[�c�܂ł̋���
	static const float partsRadius;

	//�p�[�c���Ƃ̌ŗL�ԍ�(1�`�p�[�c�̌�)
	int partsID = 0;

	//�p�[�c���Ƃ�HP
	std::vector<std::unique_ptr<SingleSprite>> partsHitPoint;

	//�p�x(�ɍ��W�p)
	float angleTheta = 0.0f;
	float anglePhi = 0.0f;

	float angleThetaRad = 0.0f;
	float anglePhiRad = 0.0f;
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
	static void uniteBossStaticInit(Player* player);

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
	void uniteBossArrival();

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

	//�o�ꉉ�o�t���O�̎擾
	bool getIsAppear() { return isAppear; }


	//�p�[�c����
	std::list<std::unique_ptr<uniteParts>> partsList;
private:

	//���f��
	static std::unique_ptr<Model> uniteBossModel;

	//�p�[�c�̐�
	static const int unitNum = 6;

	//�v���C���[�̃A�h���X
	static Player* playerPointer;

	//�p�[�c�̋��ʍ��W��ݒ肷��Ƃ��̃C���f�b�N�X(�p�x{��,��})
	std::list<XMFLOAT2> defaultPartsAngle =
	{
		{  0.0f,  0.0f},
		{ 90.0f, 90.0f},
		{180.0f,180.0f},
		{180.0f,270.0f},
		{ 90.0f,  0.0f},
		{180.0f,180.0f}
	};

	//HP�Q�[�W
	std::vector<std::unique_ptr<SingleSprite>> motherHitPoint;

	//�U���p�^�[���ꗗ
	enum class uniteBossPattern
	{
		shotPlayerTarget = 0,
		chargeAttack = 1,
		rampageBullet = 2
	};

	//���݂̍U���p�^�[��
	uniteBossPattern uniteBossNowPattern;

	/// <summary>
	/// �U���ڕW
	/// <para>�ːi�F�˂����ޏꏊ</para>
	/// <para>�ˌ�(���@�_��)�F�v���C���[�̈ʒu</para>
	/// <para>���ˁF�x�N�g�� [ �{��->�p�[�c ] �̉�������</para>
	/// </summary>
	XMFLOAT3 targetPos;

	XMFLOAT3 uniteBossScale;

	//�S�Ẵp�[�c����ꂽ���ǂ���
	bool isAllPartsBreak = false;

	//�s���p�^�[����I�𒆂��ǂ���
	bool isSelectPattern = false;

	int resetHitPoint = 11;
	int arrivalTime = 0;

	//�ːi�̉�
	int chargeAttackCount = 0;
	//�ːi�̍ő吔
	const int maxChargeAttackCount = 3;
	//�ːi�������
	XMFLOAT3 chargeAttackVec = {};

	//�e����������
	int shotCount = 0;
	//�e�����ő吔(���@�_��)
	const int maxShotCount = 5;
	//�e�����ő吔(����)
	const int maxRanpageCount = 3;

	//���o�p���W
	XMFLOAT3 arrivalStartPos;
	XMFLOAT3 arrivalEndPos;

	//���ĉ��o�p�J�����̉�]�p���x
	float CameraAngleSpeed = 0.0f;

	//�{�X���o�p�̃J����
	Camera* uniteBossCamera;
};