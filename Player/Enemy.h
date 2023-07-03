#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"../Particle/particleManagerOnTime.h"
#include"NormalBullet.h"

#include<memory>
#include<list>

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
	void changePattern(enemyPattern pattern);

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

	/// <summary>
	/// ミニマップ描画
	/// </summary>
	/// <param name="directx">directX_Bace</param>
	void drawMiniMapIcon(directX* directx);

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
	object3dFBX* enemyObject = nullptr;

	//ミニマップの基準座標
	static const XMFLOAT3 miniMapPosition;

	//プレイヤーの索敵ライン
	static const float forPlayer;
	static XMFLOAT3 playerPosition;
	static XMFLOAT3 playerFront;
	static bool playerIsAlive;

	static const XMFLOAT2 defaultRockIconSize;
	static const XMFLOAT2 setRockIconSize;
	static const float decreaseSize;

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

	//色
	XMFLOAT4 bodyColor = { 1,1,1,1 };

	//座標・初期位置・スケール・速度・回転
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float scale = 1.0f;
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};

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

	//弾
	std::list<std::unique_ptr<NormalBullet>> normalBullets;

	bool isRampageWait = true;
	int rampageWaitCount = 0;
	int bulletCount = 0;
	int nextBulletTime = 0;

	int maxBulletCount = 5;

	//撃墜エフェクト
	std::list<std::unique_ptr<SingleParticle>> bomParticles;	//爆発
	std::list<std::unique_ptr<SingleParticle>> smokeParticles;	//煙

	//パーティクルの数(なるべくこれを基準に考える)
	const int PublicParticlenum = 10;

	const int maxFallCount = 90;

	//一定距離下に落ちて行くためのカウント
	int fallDownCount = 0;

	float deathRotSpeed = 1.0f;

	/// <summary>
	/// ミニマップ上のアイコン
	/// </summary>
	SingleSprite miniMapEnemy;
	SingleSprite enemyHeight;
};