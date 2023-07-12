#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"../Particle/particleManagerOnTime.h"
#include"../Player/NormalBullet.h"

#include<memory>
#include<list>
#include<random>

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
	static void setBossCamera(XMFLOAT3 eye, XMFLOAT3 target);

	virtual void init() = 0;

	virtual void set(XMFLOAT3 pos) = 0;

	virtual void updata() = 0;

	virtual void arrival() = 0;

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
	static Camera* bossCamera;

	//�ÓI�ł͂Ȃ����ʕϐ�
	//�o�ꉉ�o����
	int enemyArrivalTime;
	int enemyArrivaCount;

	//�o�ꉉ�o�p�ϐ�
	easingManager arrivalEase;
	XMFLOAT3 arrivalScale = { 1,1,1 };

	//�I�u�W�F�N�g
	std::unique_ptr<object3dFBX> enemyObject;

	bool isDraw = false;		//�`��t���O
	bool isAlive = false;		//�����t���O
	bool isTargetSet = false;	//�_���Ă��邩�ǂ���
	bool isSetMissile = false;	//�~�T�C���������ɃZ�b�g����Ă��邩
	bool isOutScreen = false;	//��ʊO�ɂ��邩�ǂ���
	bool isFar = false;			//�`����E�t���O
	bool isStop = false;		//��~�t���O
	bool isAppear = false;		//�o�ꉉ�o�t���O

	//���ăG�t�F�N�g
	std::list<std::unique_ptr<SingleParticle>> bomParticles;	//����
	std::list<std::unique_ptr<SingleParticle>> smokeParticles;	//��

	//�p�[�e�B�N���̐�(�Ȃ�ׂ��������ɍl����)
	const int PublicParticlenum = 10;
	const int maxFallCount = 90;

	//��苗�����ɗ����čs�����߂̃J�E���g
	int fallDownCount = 0;

	float deathRotSpeed = 1.0f;

	std::unique_ptr<SingleSprite> rockTarget;//�}�[�J�[
	std::unique_ptr<SingleSprite>outScreenIcon[2];//��ʊO�A�C�R��

	/// <summary>
	/// �~�j�}�b�v��̃A�C�R��
	/// </summary>
	std::unique_ptr<SingleSprite> miniMapEnemy;
	std::unique_ptr<SingleSprite> enemyHeight;

	//�G�̓����蔻��
	Sphere enemyCollision;

	//���̓G���{�X���ǂ���
	bool isThisBoss = false;

	//���̓G�ɍU�����ʂ邩�ǂ���
	bool isArmor = false;

	//�̗�
	int HP = 1;

	//���W�E�����ʒu�E�X�P�[���E���x�E��]
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float scale = 1.0f;
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};

	//�̂̐F
	XMFLOAT4 bodyColor = { 1,1,1,1 };
	
	//�v���C���[�̌����Ă�������Ǝ����̈ʒu�Ƃ̊p�x
	float toPlayerAngle = 0.0f;

	//�G�̍s���p�^�[��
	enemyType myEnemyType = enemyType::tutorial;
};