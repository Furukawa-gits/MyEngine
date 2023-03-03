#pragma once
#include "Boss.h"

class uniteParts
	:public Enemy
{
public:
	void initParts(int ID);

	void setStaticData(XMFLOAT3* motherposition);

	void updataParts();

	void setParts(XMFLOAT3 position, float theta, float phi);

private:

	static float defaultPosition;

	//パーツごとの固有番号(1～パーツの個数)
	int partsID = 0;

	//パーツごとのHP
	std::list<std::unique_ptr<SingleSprite>> partsHitPoint;

	//パーツの座標
	XMFLOAT3 partsPosition = { 0,0,0 };

	float angleTheta;
	float anglePhi;
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
	static void uniteBossStaticInit();

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
	void uniteBossArrival(Player* player);

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

private:

	//モデル
	static std::unique_ptr<Model> uniteBossModel;

	//パーツの数
	static const int unitNum = 6;

	//プレイヤーのアドレス
	static Player* player;

	//本体からパーツまでの距離
	static const float partsRadius;

	//パーツたち
	std::list<std::unique_ptr<uniteParts>> partsList;

	//パーツの球面座標を設定するときのインデックス
	XMFLOAT2 defaultPartsAngle[6] =
	{
		{0,0},
		{90,0},
		{180,0},
		{270,0},
		{0,90},
		{0,180}
	};

	//HPゲージ
	std::list<std::unique_ptr<SingleSprite>> motherHitPoint;

	//攻撃パターン一覧
	enum class uniteBossPattern
	{
		shotPlayerTarget = 0,
		chargeAttack = 1,
		rampageBullet = 2
	};

	//現在の攻撃パターン
	uniteBossPattern nowPattern;

	/// <summary>
	/// 攻撃目標
	/// <para>突進：突っ込む場所</para>
	/// <para>射撃(自機狙い)：プレイヤーの位置</para>
	/// <para>乱射：ベクトル [ 本体->パーツ ] の延長線上</para>
	/// </summary>
	XMFLOAT3 targetPos;

	//全てのパーツが壊れたかどうか
	bool isAllPartsBreak = false;

	//演出用変数
	XMFLOAT3 arrivalStartPos;
	XMFLOAT3 arrivalEndPos;

	//ボス演出用のカメラ
	Camera* bossCamera;
};