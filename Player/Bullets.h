#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"Enemy.h"

//通常弾
class bullet
{
public:
	float count = 0;
	XMFLOAT3 bullet_vec = { 0.0f,0.0f,0.0f };

	Object3d_FBX* bulletObject = nullptr;
	Model* bulletModel = nullptr;
	float bulletSpeed = 3.0f;

	bool isArive = false;

	Sphere bullet_collision;

	bullet();
	~bullet();

	void init(int index);
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void checkHitEnemy(Enemy* enemy);
	void checkHitEnemyBullet(Enemy* enemy);
	void update();
	void draw(directX* directx);
};


//ホーミングミサイル
class Missile
{
public:
	Enemy* P_enemy = nullptr;
	XMFLOAT3 bullet_vec = { 0.0f,0.0f,0.0f };

	Object3d_FBX* bulletObject = nullptr;
	Model* bulletModel = nullptr;
	bool Isarive = false;
	bool IsTarget_set = false;
	
	Sphere missile_collision;

	XMFLOAT3 bullet_vec_index[8] = {
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

	void init(int index);
	void setPenemy(Enemy* enemy);
	void start(XMFLOAT3 start_pos);
	void checkhit();
	void update();
	void draw(directX* directx);
};