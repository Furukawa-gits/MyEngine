#pragma once
#include"../2D/Sprite.h"
#include"../3D/3Dobject.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"

class enemyBullet
{
public:
	int count = 0;
	XMFLOAT3 bulletVec = { 0.0f,0.0f,0.0f };
	bool isArive = false;
	Sphere bulletCollision;

	enemyBullet();
	~enemyBullet();
	void init(int index);
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void update();
	void draw();
};

class Enemy
{
public:
	bool isDraw = true;//描画フラグ
	bool Isarive = false;//生存フラグ
	bool isTargetSet = false;//狙われているかどうか
	bool isSetMissile = false;//ミサイルが自分にセットされているか
	SingleSprite rockTarget;//マーカー
	Sphere enemyCollision;

	//体力
	int HP = 1;
	//座標・速度
	XMFLOAT3 startPosition = {};
	XMFLOAT3 position = {};
	XMFLOAT3 rot = {};
	float enemySpeed = 0.05f;
	//オブジェクト
	Model* testCube = nullptr;
	Object3d_FBX* testObject = nullptr;
	//自機に向かっていくカウント
	int homingCount = 0;

	Enemy();

	~Enemy();

	void init();

	void set(XMFLOAT3 pos);

	void reSet();

	void update(XMFLOAT3 Player_pos);

	void chase(XMFLOAT3 pPos);

	void isHitTarget(XMFLOAT2 targetpos,bool istarget);

	void isHitShot(XMFLOAT2 targetpos);

	void draw3D(directX* directx);

	void draw2D(directX* directx);
};