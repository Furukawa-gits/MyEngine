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

	/// <summary>
	/// 敵追加
	/// <para>0:チュートリアル</para>
	/// <para>1:追跡のみ</para>
	/// <para>2:単発射撃</para>
	/// <para>3:追跡＆射撃</para>
	/// <para>4:乱射撃</para>
	/// <para>10:通常ボス</para>
	/// <para>100:ユニットボス</para>
	/// </summary>
	/// <param name="addtype">敵タイプ</param>
	void addUsuallyEnemy(enemyType addtype);

	void update();

	void draw3D();

	void draw2D();

	void drawUIs();

	std::list<unique_ptr<primitiveEnemy>>& getEnemyList() { return enemyList; }

private:
	std::list<unique_ptr<primitiveEnemy>> enemyList;

};