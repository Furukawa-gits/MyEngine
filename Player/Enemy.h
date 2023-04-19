#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"../Particle/Particle.h"

#include<memory>
#include<list>

//�G�̒e
class enemyBullet
{
public:
	enemyBullet();
	~enemyBullet();

	static void staticInit();

	static void staticDestroy();

	void init();

	void set(XMFLOAT3 playerpos, XMFLOAT3 shotpos);

	void updata();

	void draw(directX* directx);

	bool isBulletArive()
	{
		return isAlive;
	}

	Sphere getCollision()
	{
		return bulletCollision;
	}

	//�����t���O
	bool isAlive = false;

	//��������
	int ariveTime = 0;

	const int maxAriveTime = 200;

	//�����蔻��
	Sphere bulletCollision;


	/// <summary>
	/// �e�̖{�̂ƂȂ�p�[�e�B�N��
	/// <para>�e�̐����Ɉˑ�����̂ł����͎��Ԍo�߂ŏ����Ȃ�</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	/// <summary>
	/// �c���p�[�e�B�N��
	/// <para>������͒e�̐������ԂɊ֌W�Ȃ��̂Ŏ��Ԍo�߂ŏ�����</para>
	/// </summary>
	std::list<std::unique_ptr<SingleParticle>> childParticles;

private:
	//���W�E�����E�e��
	XMFLOAT3 position = {};
	XMFLOAT3 bulletVec = {};
	float bulletSpeed = 0.9f;
	XMFLOAT3 rot = {};
};

//�G�̍s���p�^�[��
enum class enemyPattern
{
	tutorial = 0,
	chase = 1,
	shot = 2,
	homing = 3,
	rampage = 4,
};

//�G
class Enemy
{
public:
	Enemy();

	~Enemy();

	static void staticInit();
	static void staticUpdata(XMFLOAT3 playerpos, XMFLOAT3 playerfront,bool playerisarive);
	static void staticDestroy();

	//������
	void init(enemyPattern pattern);

	/// <summary>
	/// �Z�b�e�B���O
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void set(XMFLOAT3 pos);

	/// <summary>
	/// �s���p�^�[���ύX
	/// <param name="pattern">�s���p�^�[��</param>
	/// </summary>
	void changePattern(enemyPattern pattern);

	//���Z�b�g
	void reSet();

	/// <summary>
	/// �X�V
	/// </summary>
	void updata();

	/// <summary>
	/// �X�v���C�g�̍X�V
	/// </summary>
	void updataSprite();

	/// <summary>
	/// �`��(3d�I�u�W�F�N�g)
	/// </summary>
	/// <param name="directx">directX_Bace</param>
	void draw3D(directX* directx);

	/// <summary>
	/// �`��(2d�I�u�W�F�N�g)
	/// </summary>
	/// <param name="directx">directX_Bace</param>
	void draw2D(directX* directx);

	/// <summary>
	/// �~�j�}�b�v�`��
	/// </summary>
	/// <param name="directx">directX_Bace</param>
	void drawMiniMapIcon(directX* directx);

private:
	/// <summary>
	/// �o��
	/// </summary>
	void arrival();

	/// <summary>
	/// �������̏���
	/// </summary>
	void ariveMove();

	//���Ď��̏���
	void deathMove();

	/// <summary>
	/// �p�^�[���O�F�`���[�g���A��
	/// </summary>
	void tutorial();

	/// <summary>
	/// �p�^�[���P�F�ǔ�
	/// </summary>
	void chase();

	/// <summary>
	/// �p�^�[���Q�F�ˌ�
	/// </summary>
	void shot();

	/// <summary>
	/// �p�^�[���R�F�ǔ����ˌ�
	/// </summary>
	void homing();

	/// <summary>
	/// �p�^�[���S�F�e����
	/// </summary>
	void rampage();

private:
	//���f��
	static std::unique_ptr<Model> enemyModelS;

	//�o�ꉉ�o����
	int enemyArrivalTime;
	int enemyArrivaCount;

	//�o�ꉉ�o�p�ϐ�
	easingManager arrivalEase;
	XMFLOAT3 arrivalScale = { 1,1,1 };

public:
	//�I�u�W�F�N�g
	object3dFBX* enemyObject = nullptr;

	//�~�j�}�b�v�̊���W
	static const XMFLOAT3 miniMapPosition;

	//�v���C���[�̍��G���C��
	static const float forPlayer;
	static XMFLOAT3 playerPosition;
	static XMFLOAT3 playerFront;
	static bool playerIsAlive;

	static const XMFLOAT2 defaultRockIconSize;
	static const XMFLOAT2 setRockIconSize;
	static const float decreaseSize;

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
	enemyPattern enemyMovePattern = enemyPattern::chase;

	//�p�^�[���P�F�ǔ�
	bool isChase = false;//�ǐՃt���O
	bool isWait = false;//�ҋ@�t���O
	int chaseCount = 0;//�ǔ��J�E���g
	int waitCount = 0;//�ҋ@�J�E���g

	//�p�^�[���Q�F�ˌ�
	bool isShot = false;//�ˌ��t���O
	int nextShotTime = 0;//���̎ˌ��܂ł̑ҋ@����
	bool isInRange = false;//�˒��͈͓����ǂ���

	//���̓G���{�X���ǂ���
	bool isThisBoss = false;

	//���̓G�ɍU�����ʂ邩�ǂ���
	bool isArmor = false;

	//�e(���ˁFrampage�Ŏg��)
	std::list<std::unique_ptr<enemyBullet>> Bullets;

	bool isRampageWait = true;
	int rampageWaitCount = 0;
	int bulletCount = 0;
	int nextBulletTime = 0;

	int maxBulletCount = 5;

	//���ăG�t�F�N�g
	std::list<std::unique_ptr<SingleParticle>> bomParticles;	//����
	std::list<std::unique_ptr<SingleParticle>> smokeParticles;	//��

	//�p�[�e�B�N���̐�(�Ȃ�ׂ��������ɍl����)
	const int PublicParticlenum = 10;

	const int maxFallCount = 90;

	//��苗�����ɗ����čs�����߂̃J�E���g
	int fallDownCount = 0;

	float deathRotSpeed = 1.0f;

	/// <summary>
	/// �~�j�}�b�v��̃A�C�R��
	/// </summary>
	SingleSprite miniMapEnemy;
};