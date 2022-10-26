#pragma once
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

	void checkPlayerBullet(Enemy* enemy);

	void checkPlayerEnemy(Enemy* enemy);

	void checkPlayerEnemyBullet(Enemy* enemy);

	void update();

	void targetUpdate();

	void reset();

	void draw3D(directX* directx);

	void draw2D(directX* directx);


	//3d�I�u�W�F�N�g
	Object3d_FBX* playerObject = nullptr;
	Model* playerModel = nullptr;

	//�Ǐ]�J����
	FollowCamera* followCamera = nullptr;

	//��]��
	float yowRotateSpeedPositive = 0.0f;//���[��](������)
	float yowRotateSpeedNegative = 0.0f;//���[��](������)
	float pitchRotateSpeedPositive = 0.0f;//�s�b�`��](������)
	float pitchRotateSpeedNegative = 0.0f;//�s�b�`��](������)
	float addRotateSpeed = 0.002f;//���Z��
	float subRotateSpeed = -0.002f;//���Z��
	float limitRotateSpeed = 0.02f;//�ő��]���x

	//���]��
	const XMFLOAT3 unitX = { 1,0,0 };
	const XMFLOAT3 unitY = { 0,1,0 };
	const XMFLOAT3 unitZ = { 0,0,1 };

	//�N�H�[�^�j�I����]�s��
	Quaternion qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);

	//�N�H�[�^�j�I����]�p�x
	float roll = 0.0f;
	float pitch = 0.0f;
	float yow = 0.0f;

	//���W
	XMFLOAT3 position = { 0,5,30 };

	//�ړ��X�s�[�h
	float moveSpeed = 0.3f;

	//�^�[�Q�b�g�X�v���C�g
	SingleSprite targetFirst;
	SingleSprite targetSecond;
	SingleSprite targetThird;
	int targetCount = 0;
	int rockOnCount = 0;
	bool isRockOn = false;

	//Hp�X�v���C�g
	SingleSprite hitPointUI[10];

	//�e
	bullet playerBullet[MaxPlayerBulletNum];
	Missile playerMissiale[MaxPlayerMissileNum];

	//����
	static dxinput* input;

	Sphere playerCollision;

	//����
	bool isArive = false;

	int playerHP = 10;

};