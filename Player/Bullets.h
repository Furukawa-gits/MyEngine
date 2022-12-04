#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"Enemy.h"

#include<memory>

//通常弾
class bullet
{
private:
	static std::unique_ptr<Model> bulletModelS;
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
	static void staticDestroy();
	void init();
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void update();
	void draw(directX* directx);
};


//ホーミングミサイル
class Missile
{
private:
	static std::unique_ptr<Model> MissileModelS;
public:

	Enemy* enemyPointer = nullptr;
	XMFLOAT3 bulletVec = { 0.0f,0.0f,0.0f };

	Object3d_FBX* bulletObject = nullptr;
	bool isArive = false;
	bool isTargetSet = false;
	
	Sphere missileCollision;

	XMFLOAT3 bulletVecIndex[8] = {
		{  30.0f,  0.0f,0.0f },
		{ -30.0f,  0.0f,0.0f },
		{   0.0f, 30.0f,0.0f },
		{   0.0f,-30.0f,0.0f },
		{  30.0f, 30.0f,0.0f },
		{  30.0f,-30.0f,0.0f },
		{ -30.0f, 30.0f,0.0f },
		{ -30.0f,-30.0f,0.0f }
	};

	Missile();
	~Missile();

	static void staticInit();
	static void staticDestroy();
	void init();
	void setPenemy(Enemy* enemy);
	void start(XMFLOAT3 start_pos);
	void update();
	void draw(directX* directx);
};