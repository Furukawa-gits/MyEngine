#pragma once
#include"primitiveEnemy.h"

class TutorialEnemy :
	public primitiveEnemy
{
public:
	TutorialEnemy();
	~TutorialEnemy();

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
};