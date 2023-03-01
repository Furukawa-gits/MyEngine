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

	/// <summary>
	/// 初期化
	/// </summary>
	void initUniteBoss();

	/// <summary>
	/// 更新
	/// </summary>
	void updateUniteBoss();

	/// <summary>
	/// 登場演出
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void uniteBossArrival(Player* player);

	/// <summary>
	/// uniteboss専用撃破演出
	/// </summary>
	void uniteBossDeathMove();

	/// <summary>
	/// セット
	/// </summary>
	void setUniteBoss();

	/// <summary>
	/// 行動パターン抽選
	/// </summary>
	void selectMovePattern();

	/// <summary>
	/// 描画(3d)
	/// </summary>
	/// <param name="directx"></param>
	void drawUniteBoss3d(directX* directx);

	/// <summary>
	/// 描画(2d)
	/// </summary>
	/// <param name="directx"></param>
	void drawUniteBoss2d(directX* directx);

private:

	//パーツの数
	static const int unitNum = 6;

	//パーツたち
	std::list<std::unique_ptr<uniteParts>> partsList;

	//本体からパーツまでの距離
	static const float partsRadius;

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

	//各HP
	std::list<std::unique_ptr<SingleSprite>> motherHitPoint;

	//全てのパーツが壊れたかどうか
	bool isAllPartsBreak = false;

	//演出用変数
	XMFLOAT3 arrivalStartPos;
	XMFLOAT3 arrivalEndPos;

	//ボス演出用のカメラ
	Camera* bossCamera;
};