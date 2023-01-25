#pragma once
#include<memory>
#include<list>
#include"../3D/Collision.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"
#include"Bullets.h"


const int MaxPlayerBulletNum = 20;
const int MaxPlayerMissileNum = 8;

class Player
{
public:
	Player();
	~Player();

	void init(dxinput* input, directX* directx);

	void Move();

	void cameraMove();

	void update();

	void targetUpdate();

	void addMissile(Enemy* enemy);

	void reset();

	void draw3D(directX* directx);

	void draw2D(directX* directx,int targetnum);

private:
	static std::unique_ptr<Model> playerModel;
	//�N�H�[�^�j�I����]�p�x
	float roll = 0.0f;
	float pitch = 0.0f;
	float yow = 0.0f;

	//���f���ʃN�H�[�^�j�I����]
	float modelRoll = 0.0f;
	float modelPitch = 0.0f;

	//�ړ��X�s�[�h
	float moveSpeed = 0.3f;

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

	//�^�[�Q�b�g�X�v���C�g
	SingleSprite targetFirst;
	int targetCount = 0;
	int rockOnCount = 0;
	bool isRockOn = false;

	//�N�H�[�^�j�I����]�s��
	Quaternion qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);

	//Hp�X�v���C�g
	std::vector<SingleSprite> HPUI[10];

	//�_���[�W�\��
	SingleSprite damage;

	//�e
	std::list<std::unique_ptr<bullet>> bulletsList;
	std::list<std::unique_ptr<Missile>> missilesList;

	//����
	static dxinput* input;

	Sphere playerCollision;

	//����
	bool isArive = false;

	int playerHP = 10;

	//���G���ԃt���O
	bool isArmor = false;
	int armorTime = 0;

	//�`�悵�Ȃ��t���O
	int isInvisible = -1;

	//��~�t���O
	bool isStop = false;

	//�J����������������
	int cameraMoveCount = 0;
};