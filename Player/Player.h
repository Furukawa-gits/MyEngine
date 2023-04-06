#pragma once
#include<memory>
#include<list>
#include"../3D/Collision.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"
#include"Bullets.h"


const int MaxPlayerMissileNum = 8;

class Player
{
public:
	Player();
	~Player();

	void init(dxinput* input, directX* directx);

	void Move();

	void outArea();

	void boostMove();

	void cameraMove();

	void playerClearMove();

	void playerDeathMove();

	void setStaging(bool isclear);

	void update();

	void targetUpdate();

	void addMissile(Enemy* enemy);

	void reset();

	void draw3D(directX* directx);

	void draw2D(directX* directx, int targetnum);

	//�v���C���[�̈ʒu���擾
	XMFLOAT3 getPlayerPos() { return playerObject->getPosition(); }

	//�v���C���[�̐��ʃx�N�g�����擾
	XMFLOAT3 getPlayerFront() { return followCamera->getFrontVec(); }

private:
	static std::unique_ptr<Model> playerModel;

	//�N�H�[�^�j�I����]�s��
	Quaternion qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);
	Quaternion qLocalCamera = quaternion(XMFLOAT3(0, 0, 1), 0);

	//�N�H�[�^�j�I����]�p�x
	float roll = 0.0f;
	float pitch = 0.0f;
	float yow = 0.0f;

	//roll��]����
	float totalRoll = 0.0f;

	//�ړ��X�s�[�h
	float moveSpeed = 0.0f;
	const float defaultMoveSpeed = 0.4f;
	const float boostMoveSpeed = 2.5f;

	SingleSprite targetSecond;
	SingleSprite targetThird;

	SingleSprite remainingMissileNum[9];

	float yowRotateSpeedPositive = 0.0f;//���[��](������)
	float yowRotateSpeedNegative = 0.0f;//���[��](������)
	float pitchRotateSpeedPositive = 0.0f;//�s�b�`��](������)
	float pitchRotateSpeedNegative = 0.0f;//�s�b�`��](������)
	float addRotateSpeed = 0.002f;//���Z��
	float subRotateSpeed = -0.002f;//���Z��
	float limitRotateSpeed = 0.02f;//�ő��]���x

	//�}�E�X�J�[�\���Œ�l
	const int mouseOffsetX = 640;
	const int mouseOffsetY = 360;

	//���]��
	const XMFLOAT3 unitX = { 1,0,0 };
	const XMFLOAT3 unitY = { 0,1,0 };
	const XMFLOAT3 unitZ = { 0,0,1 };

	const int maxArmorTime = 300;

public:
	//3d�I�u�W�F�N�g
	Object3d_FBX* playerObject = nullptr;

	//�Ǐ]�J����
	FollowCamera* followCamera = nullptr;

	//��O����p�ϐ�
	const XMFLOAT3 groundPosition = { 0,-180,0 };		//�n�ʂ̍��W(�����蔻��p)
	float lengthForPlayerPosition;						//���_����̋���
	bool isOutArea = false;								//�G���A�O�ɂ��邩�ǂ���
	int outAreaCount = 0;								//�G���A�O�ɂ��鎞��(�߂�ƃ��Z�b�g)
	bool isCautionDraw = false;							//�G���A�O�x���̕`��t���O
	SingleSprite outAreaCaution;						//�G���A�O�x��

	//�^�[�Q�b�g�X�v���C�g
	SingleSprite targetFirst;
	int targetCount = 0;
	int rockOnCount = 0;
	bool isRockOn = false;

	//Hp�X�v���C�g
	std::vector<SingleSprite> HPUI[10];

	//HP�Q�[�W
	SingleSprite HPGaugeBar;
	SingleSprite dangarHPGaugeBar;
	SingleSprite boostGaugeBar;
	SingleSprite gaugeFrame;

	//�W�F�b�g�G���W���G�t�F�N�g
	std::list<std::unique_ptr<SingleParticle>> moveParticles;
	int moveParticlesCount = 0;

	//�_���[�W�\��
	SingleSprite damage;

	//���ăG�t�F�N�g
	std::list<std::unique_ptr<SingleParticle>> bomParticles;	//����
	std::list<std::unique_ptr<SingleParticle>> smokeParticles;	//��

	//�e
	std::list<std::unique_ptr<bullet>> bulletsList;
	std::list<std::unique_ptr<Missile>> missilesList;

	//����
	static dxinput* input;

	//�����蔻��
	Sphere playerCollision;

	//�����t���O
	bool isArive = false;

	//HP
	int playerHP = 10;
	const int maxHP = 10;

	//HP���댯�ɂȂ������̌x��
	bool isDangerHP = false;
	int cautionHPCount = 0;

	//�u�[�X�g
	bool isBoost = false;
	float boostGauge = 0;
	const float maxBoostGauge = 300;

	//���G���ԃt���O
	bool isArmor = false;
	int armorTime = 0;

	//�`�悵�Ȃ��t���O
	int isInvisible = -1;

	//��~�t���O
	bool isStop = false;

	//���o�Z�b�g�t���O
	bool isStagingSet = false;

	//�N���A���o����
	bool isClearStaging = false;
	int clearTime = 0;
	const int maxClearTime = 200;

	//�Q�[���I�[�o�[���o
	bool isOverStaging = false;
	int fallCount = 0;
	const int maxFallCount = 360;
	XMFLOAT3 fallRot = { 0,0,0 };
	XMFLOAT3 fallScale = { 0,0,0 };

	//�`���[�g���A���p�ϐ�
	//�t���O
	bool isBoostTutorial = false;
	bool isNormalShot = false;
	bool isHomingMissile = false;
	//�J�E���g
	int cameraMoveCount = 0;//�J����������������
	int normalShotCount = 0;//�ʏ�e����������
	int missileCount = 0;	//�~�T�C������������
	int boostCount = 0;		//����������
};