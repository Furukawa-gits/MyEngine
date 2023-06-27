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
	//静的変数
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

	//静的ではない共通変数
	//登場演出時間
	int enemyArrivalTime;
	int enemyArrivaCount;
	//登場演出用変数
	easingManager arrivalEase;
	XMFLOAT3 arrivalScale = { 1,1,1 };
	//オブジェクト
	object3dFBX* enemyObject = nullptr;
	bool isDraw = false;//描画フラグ
	bool isAlive = false;//生存フラグ
	bool isTargetSet = false;//狙われているかどうか
	bool isSetMissile = false;//ミサイルが自分にセットされているか
	bool isOutScreen = false;//画面外にいるかどうか
	std::unique_ptr<SingleSprite> rockTarget;//マーカー
	std::unique_ptr<SingleSprite>outScreenIcon[2];//画面外アイコン
	Sphere enemyCollision;//敵の当たり判定
	//体力
	int HP = 1;
	//停止フラグ
	bool isStop = false;
	//登場演出フラグ
	bool isAppear = false;
	//座標・初期位置・スケール・速度・回転
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float scale = 1.0f;
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};
	XMFLOAT3 outScreenPos = {};
	bool isFar = false;
	//プレイヤーの向いている方向と自分の位置との角度
	float toPlayerAngle = 0.0f;
	//敵の行動パターン
	enemyType enemyMovePattern = enemyType::tutorial;
	//弾
	std::list<std::unique_ptr<NormalBullet>> Bullets;
};