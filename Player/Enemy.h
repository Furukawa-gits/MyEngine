#pragma once
#include"../2D/Sprite.h"
#include"../3D/3Dobject.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"

//�G�̒e
class enemyBullet
{
public:
	//���f��
	Model* enemyBulletModel = nullptr;
	Object3d_FBX* enemyBulletObject = nullptr;

	//
	bool isArive = false;
	int count = 0;
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bulletVec = { 0.0f,0.0f,0.0f };
	Sphere enemyBulletCollision;

	enemyBullet();
	~enemyBullet();
	void init(int index);
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void update();
	void draw();
};

//�G�̍s���p�^�[��
enum class enemyPattern
{
	chase = 1,
	shot = 2
};

//�G
class Enemy
{
public:
	bool isDraw = true;//�`��t���O
	bool Isarive = false;//�����t���O
	bool isTargetSet = false;//�_���Ă��邩�ǂ���
	bool isSetMissile = false;//�~�T�C���������ɃZ�b�g����Ă��邩
	bool isChase = false;//�ǐՃt���O
	bool isWait = false;//�ҋ@�t���O
	SingleSprite rockTarget;//�}�[�J�[
	Sphere enemyCollision;//�G�̓����蔻��

	//�̗�
	int HP = 1;

	//���W�E�����ʒu�E���x�E��]
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float enemySpeed = 0.004f;
	XMFLOAT3 rot = {};

	//�I�u�W�F�N�g
	Model* testCube = nullptr;
	Object3d_FBX* testObject = nullptr;

	//���̎����Ŏ��@�Ɍ������Ă����ׂ̃J�E���g
	int chaseCount = 0;
	int waitCount = 0;

	//��苗�����ɗ����čs�����߂̃J�E���g
	int fallDownCount = 0;

	//�G�̍s���p�^�[��
	enemyPattern enemyMovePattern = enemyPattern::chase;

	Enemy();

	~Enemy();

	void init();

	void set(XMFLOAT3 pos, enemyPattern pattern);

	void reSet();

	void update(XMFLOAT3 playerPos);

	void ariveMove(XMFLOAT3 playerPos);

	void deathMove();

	void chase(XMFLOAT3 pPos);

	void shot(XMFLOAT3 pPos);

	void isHitTarget(XMFLOAT2 targetpos, bool istarget);

	void isHitShot(XMFLOAT2 targetpos);

	void draw3D(directX* directx);

	void draw2D(directX* directx);
};