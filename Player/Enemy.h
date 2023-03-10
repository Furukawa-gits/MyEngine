#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"../Particle/Particle.h"

#include<memory>
#include<list>

//敵の弾
class enemyBullet
{
public:
	enemyBullet();
	~enemyBullet();

	static void staticInit();

	static void staticDestroy();

	void init();

	void set(XMFLOAT3 playerpos, XMFLOAT3 shotpos);

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

	//生存フラグ
	bool isArive = false;

	//生存時間
	int ariveTime = 0;

	const int maxAriveTime = 200;

	//当たり判定
	Sphere bulletCollision;

private:
	//モデル・オブジェクト
	static std::unique_ptr<Model> buletModelS;
	Object3d_FBX* bulletObject = nullptr;

	//座標・方向・弾速
	XMFLOAT3 position = {};
	XMFLOAT3 bulletVec = {};
	float bulletSpeed = 0.9f;
	XMFLOAT3 rot = {};
};

//敵の行動パターン
enum class enemyPattern
{
	tutorial = 0,
	chase = 1,
	shot = 2,
	homing = 3,
	rampage = 4,
};

//敵
class Enemy
{
public:
	Enemy();

	~Enemy();

	static void staticInit();
	static void staticUpdata(XMFLOAT3 playerpos, XMFLOAT3 playerfront,bool playerisarive);
	static void staticDestroy();

	//初期化
	void init(enemyPattern pattern);

	/// <summary>
	/// セッティング
	/// </summary>
	/// <param name="pos">初期位置</param>
	void set(XMFLOAT3 pos);

	/// <summary>
	/// 行動パターン変更
	/// <param name="pattern">行動パターン</param>
	/// </summary>
	void changePattern(enemyPattern pattern) { enemyMovePattern = pattern; }

	//リセット
	void reSet();

	/// <summary>
	/// 更新
	/// </summary>
	void updata();

	/// <summary>
	/// スプライトの更新
	/// </summary>
	void updataSprite();

	/// <summary>
	/// 描画(3dオブジェクト)
	/// </summary>
	/// <param name="directx">directX_Bace</param>
	void draw3D(directX* directx);

	/// <summary>
	/// 描画(2dオブジェクト)
	/// </summary>
	/// <param name="directx">directX_Bace</param>
	void draw2D(directX* directx);

private:
	/// <summary>
	/// 出現
	/// </summary>
	void arrival();

	/// <summary>
	/// 生存時の処理
	/// </summary>
	void ariveMove();

	//撃墜時の処理
	void deathMove();

	/// <summary>
	/// パターン０：チュートリアル
	/// </summary>
	void tutorial();

	/// <summary>
	/// パターン１：追尾
	/// </summary>
	void chase();

	/// <summary>
	/// パターン２：射撃
	/// </summary>
	void shot();

	/// <summary>
	/// パターン３：追尾＆射撃
	/// </summary>
	void homing();

	/// <summary>
	/// パターン４：弾乱射
	/// </summary>
	void rampage();

private:
	//モデル
	static std::unique_ptr<Model> enemyModelS;


	//登場演出時間
	int enemyArrivalTime;
	int enemyArrivaCount;

	//登場演出用変数
	easingManager arrivalEase;
	XMFLOAT3 arrivalScale = { 1,1,1 };

public:
	//オブジェクト
	Object3d_FBX* enemyObject = nullptr;

	//プレイヤーの索敵ライン
	static const float forPlayer;
	static XMFLOAT3 playerPosition;
	static XMFLOAT3 playerFront;
	static bool playerIsArive;

	bool isDraw = false;//描画フラグ
	bool isArive = false;//生存フラグ
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

	//座標・初期位置・速度・回転
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};
	XMFLOAT3 outScreenPos = {};

	bool isFar = false;

	//プレイヤーの向いている方向と自分の位置との角度
	float toPlayerAngle = 0.0f;

	//敵の行動パターン
	enemyPattern enemyMovePattern = enemyPattern::chase;

	//パターン１：追尾
	bool isChase = false;//追跡フラグ
	bool isWait = false;//待機フラグ
	int chaseCount = 0;//追尾カウント
	int waitCount = 0;//待機カウント

	//パターン２：射撃
	bool isShot = false;//射撃フラグ
	int nextShotTime = 0;//次の射撃までの待機時間
	bool isInRange = false;//射程範囲内かどうか

	//この敵がボスかどうか
	bool isThisBoss = false;

	//この敵に攻撃が通るかどうか
	bool isArmor = false;

	//弾(1発のみ：shotで使う)
	std::unique_ptr<enemyBullet> bullet;

	//弾(乱射：rampageで使う)
	std::list<std::unique_ptr<enemyBullet>> Bullets;

	bool isRampageWait = true;
	int rampageWaitCount = 0;
	int bulletCount = 0;
	int nextBulletTime = 0;

	int maxBulletCount = 7;

	//撃墜エフェクト
	std::list<std::unique_ptr<SingleParticle>> bomParticles;	//爆発
	std::list<std::unique_ptr<SingleParticle>> smokeParticles;	//煙

	//パーティクルの数(なるべくこれを基準に考える)
	const int PublicParticlenum = 10;

	const int maxFallCount = 90;

	//一定距離下に落ちて行くためのカウント
	int fallDownCount = 0;

	float deathRotSpeed = 1.0f;
};