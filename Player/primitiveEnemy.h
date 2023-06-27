#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"../Particle/particleManagerOnTime.h"
#include"NormalBullet.h"

#include<memory>
#include<list>

enum class enemyType
{
	tutorial = 0,
	chase = 1,
	shot = 2,
	homing = 3,
	rampage = 4,
};

class primitiveEnemy
{
public:
	primitiveEnemy();
	~primitiveEnemy();

	static void staticDataInit();
	static void staticDataUpdata(XMFLOAT3 playerpos, XMFLOAT3 playerfront, bool playerisarive);

	virtual void init() = 0;

	virtual void set(XMFLOAT3 pos) = 0;

	virtual void updata() = 0;

	virtual void ariveMove() = 0;

	virtual void deathMove() = 0;

	virtual void updataSprite() = 0;

	virtual void draw3D() = 0;

	virtual void draw2D() = 0;

	virtual void drawMiniMapIcon() = 0;

public:
	//�ÓI�ϐ�
	static directX* directx;
	static std::unique_ptr<Model> staticEnemyModel;
	static const XMFLOAT3 miniMapPosition;
	static const float forPlayer;
	static XMFLOAT3 playerPosition;
	static XMFLOAT3 playerFront;
	static bool playerIsAlive;
	static const XMFLOAT2 defaultRockIconSize;
	static const XMFLOAT2 setRockIconSize;
	static const float decreaseSize;

	//�ÓI�ł͂Ȃ����ʕϐ�
	//�o�ꉉ�o����
	int enemyArrivalTime;
	int enemyArrivaCount;
	//�o�ꉉ�o�p�ϐ�
	easingManager arrivalEase;
	XMFLOAT3 arrivalScale = { 1,1,1 };
	//�I�u�W�F�N�g
	object3dFBX* enemyObject = nullptr;
	bool isDraw = false;//�`��t���O
	bool isAlive = false;//�����t���O
	bool isTargetSet = false;//�_���Ă��邩�ǂ���
	bool isSetMissile = false;//�~�T�C���������ɃZ�b�g����Ă��邩
	bool isOutScreen = false;//��ʊO�ɂ��邩�ǂ���
	std::unique_ptr<SingleSprite> rockTarget;//�}�[�J�[
	std::unique_ptr<SingleSprite>outScreenIcon[2];//��ʊO�A�C�R��
	Sphere enemyCollision;//�G�̓����蔻��
	//�̗�
	int HP = 1;
	//��~�t���O
	bool isStop = false;
	//�o�ꉉ�o�t���O
	bool isAppear = false;
	//���W�E�����ʒu�E�X�P�[���E���x�E��]
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float scale = 1.0f;
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};
	XMFLOAT3 outScreenPos = {};
	bool isFar = false;
	//�v���C���[�̌����Ă�������Ǝ����̈ʒu�Ƃ̊p�x
	float toPlayerAngle = 0.0f;
	//�G�̍s���p�^�[��
	enemyType enemyMovePattern = enemyType::tutorial;
	//�e
	std::list<std::unique_ptr<NormalBullet>> Bullets;
};