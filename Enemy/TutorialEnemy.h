#pragma once
#include"primitiveEnemy.h"

class TutorialEnemy :
	public primitiveEnemy
{
public:
	TutorialEnemy();
	~TutorialEnemy();

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

	//チュートリアル用なのでボス用撃墜処理はナシ
	void deathMoveBoss() override;

	//UIスプライト更新
	void updataSprite() override;

	//3D描画
	void draw3D() override;

	//2D描画
	void draw2D() override;

	//2D描画(ミニマップ)
	void drawMiniMapIcon() override;
};