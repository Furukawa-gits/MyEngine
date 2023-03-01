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

	/// <summary>
	/// ������
	/// </summary>
	void initUniteBoss();

	/// <summary>
	/// �X�V
	/// </summary>
	void updateUniteBoss();

	/// <summary>
	/// �o�ꉉ�o
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void uniteBossArrival(Player* player);

	/// <summary>
	/// uniteboss��p���j���o
	/// </summary>
	void uniteBossDeathMove();

	/// <summary>
	/// �Z�b�g
	/// </summary>
	void setUniteBoss();

	/// <summary>
	/// �s���p�^�[�����I
	/// </summary>
	void selectMovePattern();

	/// <summary>
	/// �`��(3d)
	/// </summary>
	/// <param name="directx"></param>
	void drawUniteBoss3d(directX* directx);

	/// <summary>
	/// �`��(2d)
	/// </summary>
	/// <param name="directx"></param>
	void drawUniteBoss2d(directX* directx);

private:

	//�p�[�c�̐�
	static const int unitNum = 6;

	//�p�[�c����
	std::list<std::unique_ptr<uniteParts>> partsList;

	//�{�̂���p�[�c�܂ł̋���
	static const float partsRadius;

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

	//�eHP
	std::list<std::unique_ptr<SingleSprite>> motherHitPoint;

	//�S�Ẵp�[�c����ꂽ���ǂ���
	bool isAllPartsBreak = false;

	//���o�p�ϐ�
	XMFLOAT3 arrivalStartPos;
	XMFLOAT3 arrivalEndPos;

	//�{�X���o�p�̃J����
	Camera* bossCamera;
};