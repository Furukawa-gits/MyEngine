#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"

#include<memory>
#include<list>

//�G�̒e
class enemyBullet
{
public:
	enemyBullet();
	~enemyBullet();

	void init();

	void set(XMFLOAT3 playerpos,XMFLOAT3 shotpos);

	void update();

	void draw(directX* directx);

	bool isBulletArive()
	{
		return (bulletObject != nullptr) && isArive;
	}

	Sphere getCollision()
	{
		return bulletCollision;
	}

	//�����t���O
	bool isArive = false;

	//��������
	int ariveTime = 0;

	//�����蔻��
	Sphere bulletCollision;

private:
	//���f���E�I�u�W�F�N�g
	Model* buletModel = nullptr;
	Object3d_FBX* bulletObject = nullptr;

	//���W�E�����E�e��
	XMFLOAT3 position = {};
	XMFLOAT3 bulletVec = {};
	float bulletSpeed = 0.5f;
	XMFLOAT3 rot = {};
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
	//�I�u�W�F�N�g
	Model* enemyModel = nullptr;
	Object3d_FBX* enemyObject = nullptr;

	bool isDraw = false;//�`��t���O
	bool Isarive = false;//�����t���O
	bool isTargetSet = false;//�_���Ă��邩�ǂ���
	bool isSetMissile = false;//�~�T�C���������ɃZ�b�g����Ă��邩
	bool isChase = false;//�ǐՃt���O
	bool isWait = false;//�ҋ@�t���O
	SingleSprite rockTarget;//�}�[�J�[
	Sphere enemyCollision;//�G�̓����蔻��

	//�̗�
	int HP = 1;

	//��~�t���O
	bool isStop = false;

	//���W�E�����ʒu�E���x�E��]
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};

	//��苗�����ɗ����čs�����߂̃J�E���g
	int fallDownCount = 0;

	float deathRotSpeed = 2.0f;

	//�G�̍s���p�^�[��
	enemyPattern enemyMovePattern = enemyPattern::chase;

	//�p�^�[���P�F�ǔ�
	int chaseCount = 0;//�ǔ��J�E���g
	int waitCount = 0;//�ҋ@�J�E���g

	//�p�^�[���Q�F�ˌ�
	bool isShot = false;//�ˌ��t���O
	int shotCount = 0;//���̎ˌ��܂ł̑ҋ@����
	bool isInRange = false;//�˒��͈͓����ǂ���

	//���̓G���{�X���ǂ���
	bool isThisBoss = false;

	//�e
	enemyBullet bullet;

	Enemy();

	~Enemy();

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
	void changePattern(enemyPattern pattern) { enemyMovePattern = pattern; }

	//���Z�b�g
	void reSet();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void update(XMFLOAT3 playerPos);

	/// <summary>
	/// �������̏���
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void ariveMove(XMFLOAT3 playerPos);

	//���Ď��̏���
	void deathMove();

	/// <summary>
	/// �p�^�[���P�F�ǔ�
	/// </summary>
	/// <param name="pPos">�v���C���[�̍��W</param>
	void chase(XMFLOAT3 pPos);

	/// <summary>
	/// �p�^�[���Q�F�ˌ�
	/// </summary>
	/// <param name="pPos">�v���C���[�̍��W</param>
	void shot(XMFLOAT3 pPos);

	void isHitTarget(XMFLOAT2 targetpos, bool istarget);

	void isHitShot(XMFLOAT2 targetpos);

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
};