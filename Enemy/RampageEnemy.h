#pragma once
#include"primitiveEnemy.h"

class RampageEnemy :
	public primitiveEnemy
{
public:
	RampageEnemy();
	~RampageEnemy();

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
	void rampage();

	//���˗p
	bool isShot = false;//�ˌ��t���O
	int nextShotTime = 0;//���̎ˌ��܂ł̑ҋ@����
	bool isInRange = false;//�˒��͈͓����ǂ���

	//�e
	std::list<std::unique_ptr<NormalBullet>> normalBullets;

	bool isRampageWait = true;
	int rampageWaitCount = 0;
	int bulletCount = 0;
	int nextBulletTime = 0;

	int maxBulletCount = 5;

};