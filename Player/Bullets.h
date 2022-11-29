#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"Enemy.h"

#include<memory>

//�ʏ�e
class bullet
{
private:
	static Model* bulletModelS;
public:
	float count = 0;
	XMFLOAT3 bullet_vec = { 0.0f,0.0f,0.0f };

	Object3d_FBX* bulletObject = nullptr;
	float bulletSpeed = 3.0f;

	bool isArive = false;

	Sphere bulletCollision;

	bullet();
	~bullet();

	static void staticInit();
	void init();
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void checkHitEnemy(Enemy* enemy);
	void checkHitEnemyBullet(Enemy* enemy);
	void update();
	void draw(directX* directx);
};


//�z�[�~���O�~�T�C��
class Missile
{
private:
	static Model* bulletModelS;
public:
	std::unique_ptr<Enemy>* pEnemy;

	Enemy* enemyPointer = nullptr;
	XMFLOAT3 bulletVec = { 0.0f,0.0f,0.0f };

	Object3d_FBX* bulletObject = nullptr;
	bool isArive = false;
	bool isTargetSet = false;
	
	Sphere missileCollision;

	XMFLOAT3 bulletVecIndex[8] = {
		{  10.0f,  0.0f,0.0f },
		{ -10.0f,  0.0f,0.0f },
		{   0.0f, 10.0f,0.0f },
		{   0.0f,-10.0f,0.0f },
		{  10.0f, 10.0f,0.0f },
		{  10.0f,-10.0f,0.0f },
		{ -10.0f, 10.0f,0.0f },
		{ -10.0f,-10.0f,0.0f }
	};

	Missile();
	~Missile();

	static void staticInit();
	void init();
	void setPenemy(Enemy* enemy);
	void start(XMFLOAT3 start_pos);
	void checkhit();
	void update();
	void draw(directX* directx);
};