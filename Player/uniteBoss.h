#pragma once
#include "Boss.h"

class uniteParts
	:public Enemy
{
public:
	void partsInit(int ID);

	static void setStaticData(XMFLOAT3* motherposition);

	void partsUpdata();

	void partsSpriteUpdata();

	void partsArrival();

	void partsAriveMove();

	void partsDeathMove();

	void partsShotBullet(XMFLOAT3 targetposition);

	void partsSet(XMFLOAT3 position, float theta, float phi);

	void partsDraw3D(directX* directx);

	void partsDraw2D(directX* directx);

	//角速度
	float angleSpeed = 0.01f;

private:

	const float pi = 3.1415f;
	const float radiannum = 180.0f;

	//モデル
	static std::unique_ptr<Model> partsModel;

	//本体の座標
	static XMFLOAT3* motherPosition;

	//本体からパーツまでの距離
	static const float partsRadius;

	//パーツごとの固有番号(1～パーツの個数)
	int partsID = 0;

	//角度(極座標用)
	float angleTheta = 0.0f;
	float anglePhi = 0.0f;

	float angleThetaRad = 0.0f;
	float anglePhiRad = 0.0f;
};

class uniteBoss
	:public Enemy
{
public:
	uniteBoss();
	~uniteBoss();

	/// <summary>
	/// モデル読み込み
	/// </summary>
	static void uniteBossStaticInit(Player* player);

	/// <summary>
	/// 初期化
	/// </summary>
	void uniteBossInit();

	/// <summary>
	/// 更新
	/// </summary>
	void uniteBossUpdata();

	/// <summary>
	/// スプライト更新
	/// </summary>
	void uniteBossSpriteUpdata();

	/// <summary>
	/// セット
	/// </summary>
	void uniteBossSet();

	/// <summary>
	/// 登場演出
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void uniteBossArrival();

	/// <summary>
	/// 生存時処理
	/// </summary>
	void uniteBossAriveMove();

	/// <summary>
	/// uniteboss専用撃破演出
	/// </summary>
	void uniteBossDeathMove();

	/// <summary>
	/// 行動パターン抽選
	/// </summary>
	void selectMovePattern();

	/// <summary>
	/// プレイヤー追従
	/// </summary>
	void uniteBossChargeAttack();

	/// <summary>
	/// 射撃(乱射)
	/// </summary>
	void uniteBossRampage();

	/// <summary>
	/// 射撃(プレイヤー狙い)
	/// </summary>
	void uniteBossShotPlayerTarget();

	/// <summary>
	/// 描画(3d)
	/// </summary>
	/// <param name="directx"></param>
	void uniteBossDraw3d(directX* directx);

	/// <summary>
	/// 描画(2d)
	/// </summary>
	/// <param name="directx"></param>
	void uniteBossDraw2d(directX* directx);

	//登場演出フラグの取得
	bool getIsAppear() { return isAppear; }


	//パーツたち
	std::list<std::unique_ptr<uniteParts>> partsList;
private:

	//モデル
	static std::unique_ptr<Model> uniteBossModel;

	//パーツの数
	static const int unitNum = 6;

	//プレイヤーのアドレス
	static Player* playerPointer;

	//パーツの球面座標を設定するときのインデックス(角度{θ,φ})
	std::list<XMFLOAT2> defaultPartsAngle =
	{
		{  0.0f,  0.0f},
		{ 90.0f, 90.0f},
		{180.0f,180.0f},
		{180.0f,270.0f},
		{ 90.0f,  0.0f},
		{180.0f,180.0f}
	};

	//HPゲージ
	SingleSprite motherHitPointGauge;
	//パーツの合計HP
	SingleSprite allPartsHitPointGauge;

	//攻撃パターン一覧
	enum class uniteBossPattern
	{
		shotPlayerTarget = 0,
		chargeAttack = 1,
		rampageBullet = 2
	};

	//現在の攻撃パターン
	uniteBossPattern uniteBossNowPattern = uniteBossPattern::chargeAttack;

	/// <summary>
	/// 攻撃目標
	/// <para>突進：突っ込む場所</para>
	/// <para>射撃(自機狙い)：プレイヤーの位置</para>
	/// <para>乱射：ベクトル [ 本体->パーツ ] の延長線上</para>
	/// </summary>
	XMFLOAT3 targetPos = { 0,0,0 };

	XMFLOAT3 uniteBossScale = { 0,0,0 };

	//全てのパーツが壊れたかどうか
	bool isAllPartsBreak = false;

	//行動パターンを選択中かどうか
	bool isSelectPattern = false;

	int resetHitPoint = 7;
	int arrivalTime = 0;

	//突進の回数
	int chargeAttackCount = 0;
	//突進の最大数
	const int maxChargeAttackCount = 3;
	//突進する向き
	XMFLOAT3 chargeAttackVec = {};

	//弾を撃った数
	int shotCount = 0;
	//弾を撃つ最大数(自機狙い)
	const int maxShotCount = 5;
	//弾を撃つ最大数(乱射)
	const int maxRanpageCount = 3;

	//演出用座標
	XMFLOAT3 arrivalStartPos = { 0,0,0 };
	XMFLOAT3 arrivalEndPos = { 0,0,0 };

	//撃墜演出用カメラの回転角速度
	float CameraAngleSpeed = 0.0f;

	//ボス演出用のカメラ
	Camera* uniteBossCamera = nullptr;
};