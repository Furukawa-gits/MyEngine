#pragma once
#include"primitiveEnemy.h"

class HomingEnemy :
	public primitiveEnemy
{
public:
	HomingEnemy();
	~HomingEnemy();

	//初期化
	void init(bool isboss) override;

	//セット
	void set(XMFLOAT3 pos) override;

	//更新
	void updata() override;

	//頓所演出
	void arrival() override;

	//生存時処理
	void ariveMove() override;

	//撃墜処理
	void deathMove() override;

	//ボス専用撃墜演出(ifで片づけられないレベルで違うので撃墜だけ別)
	void deathMoveBoss() override;

	//UIスプライト更新
	void updataSprite() override;

	//3D描画
	void draw3D() override;

	//2D描画
	void draw2D() override;

	//2D描画(ミニマップ)
	void drawMiniMapIcon() override;

private:
	void homing();

	//追尾＆射撃用
	bool isChase = false;//追跡フラグ
	bool isWait = false;//待機フラグ
	int chaseCount = 0;//追尾カウント
	int waitCount = 0;//待機カウント

	//射撃用
	bool isShot = false;//射撃フラグ
	int nextShotMaxTime = 0;//次の射撃までの待機時間
	int nextShotTime = 0;//次の射撃までの待機時間
	bool isInRange = false;//射程範囲内かどうか

	//弾
	std::list<std::unique_ptr<NormalBullet>> normalBullets;
};