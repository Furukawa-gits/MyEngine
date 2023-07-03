#pragma once
#include"primitiveEnemy.h"

class ChaseEnemy :
	public primitiveEnemy
{
public:
	ChaseEnemy();
	~ChaseEnemy();

	void init() override;

	void set(XMFLOAT3 pos) override;

	void updata() override;

	void arrival() override;

	void ariveMove() override;

	void deathMove() override;

	void updataSprite() override;

	void draw3D() override;

	void draw2D() override;

	void drawMiniMapIcon() override;

private:
	void chase();

	//追尾用
	bool isChase = false;//追跡フラグ
	bool isWait = false;//待機フラグ
	int chaseCount = 0;//追尾カウント
	int waitCount = 0;//待機カウント
};