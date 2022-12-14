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

	//当たり判定
	Sphere bulletCollision;

private:
	//モデル・オブジェクト
	static std::unique_ptr<Model> buletModelS;
	Object3d_FBX* bulletObject = nullptr;

	//座標・方向・弾速
	XMFLOAT3 position = {};
	XMFLOAT3 bulletVec = {};
	float bulletSpeed = 0.5f;
	XMFLOAT3 rot = {};
};

//敵の行動パターン
enum class enemyPattern
{
	chase = 1,
	shot = 2,
};

//敵
class Enemy
{
public:
	//オブジェクト
	static std::unique_ptr<Model> enemyModelS;
	Object3d_FBX* enemyObject = nullptr;

	//プレイヤーの索敵ライン
	static const float forPlayer;

	bool isDraw = false;//描画フラグ
	bool Isarive = false;//生存フラグ
	bool isTargetSet = false;//狙われているかどうか
	bool isSetMissile = false;//ミサイルが自分にセットされているか
	bool isChase = false;//追跡フラグ
	bool isWait = false;//待機フラグ
	std::unique_ptr<SingleSprite> rockTarget;//マーカー
	Sphere enemyCollision;//敵の当たり判定

	//体力
	int HP = 1;

	//停止フラグ
	bool isStop = false;

	//座標・初期位置・速度・回転
	XMFLOAT3 position = {};
	XMFLOAT3 startPosition = {};
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};

	//一定距離下に落ちて行くためのカウント
	int fallDownCount = 0;

	const int maxFallCount = 90;

	float deathRotSpeed = 1.0f;

	bool isFar = false;

	//敵の行動パターン
	enemyPattern enemyMovePattern = enemyPattern::chase;

	//パターン１：追尾
	int chaseCount = 0;//追尾カウント
	int waitCount = 0;//待機カウント

	//パターン２：射撃
	bool isShot = false;//射撃フラグ
	int shotCount = 0;//次の射撃までの待機時間
	bool isInRange = false;//射程範囲内かどうか

	//この敵がボスかどうか
	bool isThisBoss = false;

	//弾
	std::unique_ptr<enemyBullet> bullet;

	//爆発エフェクト
	std::list<std::unique_ptr<SingleParticle>> particles;

	//パーティクルの数(なるべくこれを基準に考える)
	const int PublicParticlenum = 10;

	Enemy();

	~Enemy();

	static void staticInit();
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
	/// <param name="playerPos">プレイヤーの座標</param>
	void update(XMFLOAT3 playerPos);

	/// <summary>
	/// 生存時の処理
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void ariveMove(XMFLOAT3 playerPos);

	//撃墜時の処理
	void deathMove();

	/// <summary>
	/// パターン１：追尾
	/// </summary>
	/// <param name="pPos">プレイヤーの座標</param>
	void chase(XMFLOAT3 pPos);

	/// <summary>
	/// パターン２：射撃
	/// </summary>
	/// <param name="pPos">プレイヤーの座標</param>
	void shot(XMFLOAT3 pPos);

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
};