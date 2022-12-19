#pragma once
#include"Player.h"
#include"../staging/Easing.h"

class Boss
	:public Enemy
{
public:
	Boss();
	~Boss();

	/// <summary>
	/// 初期化
	/// </summary>
	void bossInit();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーのポインター</param>
	void bossUpdate(Player* player);

	/// <summary>
	/// ボスのセッティング
	/// </summary>
	/// <param name="pos">座標</param>
	void bossSet(XMFLOAT3 pos);

	/// <summary>
	/// リセット
	/// </summary>
	void bossReSet();

	/// <summary>
	/// ボスの H P のセッティング
	/// </summary>
	/// <param name="hp">体力</param>
	void setHitPoint(int hp)
	{
		HP = hp;
		resetHitPoint = hp;
		return;
	}

	/// <summary>
	/// ボスの登場演出
	/// </summary>
	/// <param name="player">プレイヤーのポインター</param>
	void bossArrival(Player* player);

	//登場演出フラグの取得
	bool getIsAppear() { return isAppear; }

private:
	//モデル
	static std::unique_ptr<Model> bossModel;

	int resetHitPoint = 3;
	int arrivalTime = 0;
	//登場演出フラグ
	bool isAppear = false;
	easingManager bossRotEase;
	XMFLOAT3 bossbaseScale;
	XMFLOAT3 bossScale;

	//ボス出現演出用のカメラ
	Camera* bossCamera;
};