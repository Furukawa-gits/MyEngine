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
	/// モデル読み込み
	/// </summary>
	static void staticInitBoss();

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
	/// スプライトの更新
	/// </summary>
	void bossSpriteUpdata();

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

	/// <summary>
	/// 生存時処理
	/// </summary>
	void bossAriveMove();

	/// <summary>
	/// 撃墜処理
	/// </summary>
	void bossDeathMove();

	/// <summary>
	/// プレイヤー追従
	/// </summary>
	void bossChase();

	/// <summary>
	/// 射撃
	/// </summary>
	void bossShot();

	/// <summary>
	/// 追尾＆射撃
	/// </summary>
	void bossHoming();

	/// <summary>
	/// 乱射
	/// </summary>
	void bossRampage();

	void bossDraw3D(directX* directx);
	void bossDraw2D(directX* directx);

	//登場演出フラグの取得
	bool getIsAppear() { return isAppear; }

	//モデル
	static std::unique_ptr<Model> bossModel;
private:

	int resetHitPoint = 3;
	int arrivalTime = 0;
	easingManager bossRotEase;
	XMFLOAT3 bossbaseScale;
	XMFLOAT3 bossScale;
	SingleSprite bossHitPointGauge;

	Player* playerPointer = nullptr;

	//ボス出現演出用のカメラ
	Camera* bossCamera;
};