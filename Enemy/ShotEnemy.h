#pragma once
#include"primitiveEnemy.h"

class ShotEnemy :
	public primitiveEnemy
{
public:
	ShotEnemy();
	~ShotEnemy();

	void init(bool isboss) override;

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
	void shot();

	//射撃用
	bool isShot = false;//射撃フラグ
	int nextShotTime = 0;//次の射撃までの待機時間
	bool isInRange = false;//射程範囲内かどうか

	//弾
	std::list<std::unique_ptr<NormalBullet>> normalBullets;

};