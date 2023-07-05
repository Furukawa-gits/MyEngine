#pragma once
#include"TutorialEnemy.h"
#include"ChaseEnemy.h"
#include"ShotEnemy.h"
#include"HomingEnemy.h"
#include"RampageEnemy.h"


class UsuallyEnemymanager
{
public:
	UsuallyEnemymanager();
	~UsuallyEnemymanager();

	void init();

	void addEnemy(enemyType addtype);

	void update();

	void draw3D();

	void draw2D();

	void drawUIs();

	std::list<std::unique_ptr<primitiveEnemy>> getEnemyList() { return enemyList; }

private:
	std::list<std::unique_ptr<primitiveEnemy>> enemyList;

};