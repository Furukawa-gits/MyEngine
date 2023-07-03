#pragma once
#include"primitiveEnemy.h"

class HomingEnemy :
	public primitiveEnemy
{
public:
	HomingEnemy();
	~HomingEnemy();

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
	void homing();

	//�ǔ����ˌ��p
	bool isChase = false;//�ǐՃt���O
	bool isWait = false;//�ҋ@�t���O
	int chaseCount = 0;//�ǔ��J�E���g
	int waitCount = 0;//�ҋ@�J�E���g

	//�ˌ��p
	bool isShot = false;//�ˌ��t���O
	int nextShotTime = 0;//���̎ˌ��܂ł̑ҋ@����
	bool isInRange = false;//�˒��͈͓����ǂ���

	//�e
	std::list<std::unique_ptr<NormalBullet>> normalBullets;
};