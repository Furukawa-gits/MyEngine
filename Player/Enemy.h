#pragma once
#include"../2D/Sprite.h"
#include"../3D/3Dobject.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"

//敵の弾
class enemyBullet
{
public:
	//モデル
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

//敵の行動パターン
enum class enemyPattern
{
	chase = 1,
	shot = 2
};

//敵
class Enemy
{
public:
	bool isDraw = true;//描画フラグ
	bool Isarive = false;//生存フラグ
	bool isTargetSet = false;//狙われているかどうか
	bool isSetMissile = false;//ミサイルが自分にセットされているか
	bool isChase = false;//追跡フラグ
	bool isWait = false;//待機フラグ
	SingleSprite rockTarget;//マーカー
	Sphere enemyCollision;//敵の当たり判定

	//体力
	int HP = 1;

	//座標・初期位置・速度・回転
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float enemySpeed = 0.004f;
	XMFLOAT3 rot = {};

	//オブジェクト
	Model* testCube = nullptr;
	Object3d_FBX* testObject = nullptr;

	//一定の周期で自機に向かっていく為のカウント
	int chaseCount = 0;
	int waitCount = 0;

	//一定距離下に落ちて行くためのカウント
	int fallDownCount = 0;

	//敵の行動パターン
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