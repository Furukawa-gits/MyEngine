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
	normalBoss = 10,
	uniteBoss = 100
};

class primitiveEnemy
{
public:
	primitiveEnemy();
	~primitiveEnemy();

	static void staticDataInit();
	static void staticDataUpdata(XMFLOAT3 playerpos, XMFLOAT3 playerfront, bool playerisarive);
	static void setUsingPlayerData(int* playerisinv,bool* playerisstop,Camera* playercamera);
	static void setBossCamera(XMFLOAT3 eye, XMFLOAT3 target);

	virtual void init(bool isboss) = 0;

	virtual void set(XMFLOAT3 pos) = 0;

	virtual void updata() = 0;

	virtual void arrival() = 0;

	virtual void ariveMove() = 0;

	virtual void deathMove() = 0;

	virtual void deathMoveBoss() = 0;

	virtual void updataSprite() = 0;

	virtual void draw3D() = 0;

	virtual void draw2D() = 0;

	virtual void drawMiniMapIcon() = 0;

	void setHitPoint(int hp)
	{
		HP = hp;
		return;
	}

public:
	//--------------------------静的変数--------------------------
	static directX* directx;
	static std::unique_ptr<Model> staticEnemyModel;
	static const XMFLOAT3 miniMapPosition;
	static const float forPlayer;
	static XMFLOAT3 playerPosition;
	static XMFLOAT3 playerFront;
	static bool playerIsAlive;
	static bool* playerIsStop;
	static int* playerIsInvisible;
	static Camera* playerCamera;
	static const XMFLOAT2 defaultRockIconSize;
	static const XMFLOAT2 setRockIconSize;
	static const float decreaseSize;
	static Camera* bossCamera;

	//--------------------------静的ではない共通変数--------------------------
	//登場演出時間
	int enemyArrivalMaxTime;
	int enemyArrivalTime;

	//登場演出用変数
	easingManager arrivalEase;
	XMFLOAT3 arrivalScale = { 1,1,1 };
	XMFLOAT3 arrivalMaxScale = { 1,1,1 };

	//オブジェクト
	std::unique_ptr<object3dFBX> enemyObject;

	bool isDraw = false;		//描画フラグ
	bool isAlive = false;		//生存フラグ
	bool isTargetSet = false;	//狙われているかどうか
	bool isSetMissile = false;	//ミサイルが自分にセットされているか
	bool isOutScreen = false;	//画面外にいるかどうか
	bool isFar = false;			//描画限界フラグ
	bool isStop = false;		//停止フラグ
	bool isAppear = false;		//登場演出フラグ

	//撃墜エフェクト
	std::list<std::unique_ptr<SingleParticle>> bomParticles;	//爆発
	std::list<std::unique_ptr<SingleParticle>> smokeParticles;	//煙

	//パーティクルの数(なるべくこれを基準に考える)
	const int PublicParticlenum = 10;
	const int maxFallCount = 90;

	//一定距離下に落ちて行くためのカウント
	int fallDownCount = 0;

	float deathRotSpeed = 1.0f;

	std::unique_ptr<SingleSprite> rockTarget;//マーカー
	std::unique_ptr<SingleSprite>outScreenIcon[2];//画面外アイコン

	/// <summary>
	/// ミニマップ上のアイコン
	/// </summary>
	std::unique_ptr<SingleSprite> miniMapEnemy;
	std::unique_ptr<SingleSprite> enemyHeight;

	//敵の当たり判定
	Sphere enemyCollision;

	//この敵がボスかどうか
	bool isThisBoss = false;

	//ボスだった場合
	std::unique_ptr<SingleSprite> bossHitPointGauge;//HP
	XMFLOAT3 bossbaseScale;							//最大サイズ

	//この敵に攻撃が通るかどうか
	bool isArmor = false;

	//体力
	int HP = 1;

	//座標・初期位置・スケール・速度・回転
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float scale = 1.0f;
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};

	//体の色
	XMFLOAT4 bodyColor = { 1,1,1,1 };

	//プレイヤーの向いている方向と自分の位置との角度
	float toPlayerAngle = 0.0f;

	//敵の行動パターン
	enemyType myEnemyType = enemyType::tutorial;
};