#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"

//敵の行動パターン
enum class enemyPattern
{
	chase = 1,
	shot = 2,
	bullet = 3
};

//敵
class Enemy
{
public:
	//オブジェクト
	Model* testCube = nullptr;
	Object3d_FBX* testObject = nullptr;

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
	float enemySpeed = 0.0f;
	XMFLOAT3 rot = {};

	//一定距離下に落ちて行くためのカウント
	int fallDownCount = 0;

	//敵の行動パターン
	enemyPattern enemyMovePattern = enemyPattern::chase;

	//パターン１：追尾
	int chaseCount = 0;//追尾カウント
	int waitCount = 0;//待機カウント

	//パターン２：射撃
	bool isShot = false;//射撃フラグ
	int shotCount = 0;//次の射撃までの待機時間

	//パターン３：弾
	int ariveTime = 0;

	Enemy();

	~Enemy();

	//初期化
	void init();

	/// <summary>
	/// セッティング
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="pattern">行動パターン</param>
	void set(XMFLOAT3 pos, enemyPattern pattern);

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

	//パターン３：弾
	void bullet();

	void isHitTarget(XMFLOAT2 targetpos, bool istarget);

	void isHitShot(XMFLOAT2 targetpos);

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