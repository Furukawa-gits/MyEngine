#pragma once
#include"primitiveEnemy.h"

class ChaseEnemy :
	public primitiveEnemy
{
public:
	ChaseEnemy();
	~ChaseEnemy();

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
	void chase();

	//追尾用
	bool isChase = false;//追跡フラグ
	bool isWait = false;//待機フラグ
	int chaseCount = 0;//追尾カウント
	int waitCount = 0;//待機カウント
};