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

void UsuallyEnemymanager::addEnemy(enemyType addtype)
{
	//unique_ptr<primitiveEnemy> newEnemy = std::make_unique<primitiveEnemy>();
	primitiveEnemy* newEnemy = nullptr;

	//ランダムで初期位置を決める
	std::random_device seed;
	std::mt19937 rnd(seed());

	std::uint32_t xResult = rnd();
	std::uint32_t yResult = rnd();
	std::uint32_t zResult = rnd();

	XMFLOAT3 startPos;

	switch (addtype)
	{
	case enemyType::tutorial:

		//newEnemy = std::move(std::make_unique<TutorialEnemy>());
		newEnemy = new TutorialEnemy;

		startPos =
		{
			(float)(xResult % 100 - 50),
			(float)(xResult % 60 + 30),
			(float)(xResult % 100 - 50)
		};
		break;

	case enemyType::chase:

		//newEnemy = std::move(std::make_unique<ChaseEnemy>());
		newEnemy = new TutorialEnemy;

		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::shot:

		//newEnemy = std::move(std::make_unique<ShotEnemy>());
		newEnemy = new TutorialEnemy;

		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::homing:

		//newEnemy = std::move(std::make_unique<HomingEnemy>());
		newEnemy = new TutorialEnemy;

		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::rampage:

		//newEnemy = std::move(std::make_unique<RampageEnemy>());
		newEnemy = new TutorialEnemy;

		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;
	}

	newEnemy->init();
	newEnemy->set(startPos);

	enemyList.push_back(newEnemy);
}

void UsuallyEnemymanager::update()
{
	enemyList.remove_if([](primitiveEnemy*& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (primitiveEnemy*& newenemy : enemyList)
	{
		newenemy->updata();
	}
}

void UsuallyEnemymanager::draw3D()
{
	for (primitiveEnemy*& newenemy : enemyList)
	{
		newenemy->draw3D();
	}
}

void UsuallyEnemymanager::draw2D()
{
	for (primitiveEnemy*& newenemy : enemyList)
	{
		newenemy->draw2D();
	}
}

void UsuallyEnemymanager::drawUIs()
{
	for (primitiveEnemy*& newenemy : enemyList)
	{
		newenemy->drawMiniMapIcon();
	}
}
