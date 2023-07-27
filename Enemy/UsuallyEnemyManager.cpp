#include "UsuallyEnemyManager.h"
#include<random>

UsuallyEnemymanager::UsuallyEnemymanager()
{
}

UsuallyEnemymanager::~UsuallyEnemymanager()
{
	enemyList.clear();
}

void UsuallyEnemymanager::init()
{
	enemyList.clear();
}

void UsuallyEnemymanager::addUsuallyEnemy(enemyType addtype)
{
	//新しく生成したenemy
	unique_ptr<primitiveEnemy> newEnemy = nullptr;

	//ランダムで初期位置を決める
	std::random_device seed;
	std::mt19937 rnd(seed());

	std::uint32_t xResult = rnd();
	std::uint32_t yResult = rnd();
	std::uint32_t zResult = rnd();

	XMFLOAT3 startPos = {};

	//生成する敵のタイプによって派生クラスを代入・初期位置を決定
	switch (addtype)
	{
	case enemyType::tutorial:

		newEnemy = std::make_unique<TutorialEnemy>();

		startPos =
		{
			(float)(xResult % 100 - 50),
			(float)(xResult % 60 + 30),
			(float)(xResult % 100 - 50)
		};
		break;

	case enemyType::chase:

		newEnemy = std::make_unique<ChaseEnemy>();

		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::shot:

		newEnemy = std::make_unique<ShotEnemy>();

		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::homing:

		newEnemy = std::make_unique<HomingEnemy>();

		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::rampage:

		newEnemy = std::make_unique<RampageEnemy>();

		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;
	}

	//マネージャーに追加
	newEnemy->init(false);
	newEnemy->set(startPos);

	enemyList.push_back(std::move(newEnemy));
}

void UsuallyEnemymanager::update()
{
	enemyList.remove_if([](unique_ptr<primitiveEnemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (unique_ptr<primitiveEnemy>& newenemy : enemyList)
	{
		newenemy->updata();
	}
}

void UsuallyEnemymanager::draw3D()
{
	for (unique_ptr<primitiveEnemy>& newenemy : enemyList)
	{
		newenemy->draw3D();
	}
}

void UsuallyEnemymanager::draw2D()
{
	for (unique_ptr<primitiveEnemy>& newenemy : enemyList)
	{
		newenemy->draw2D();
	}
}

void UsuallyEnemymanager::drawUIs()
{
	for (unique_ptr<primitiveEnemy>& newenemy : enemyList)
	{
		newenemy->drawMiniMapIcon();
	}
}
