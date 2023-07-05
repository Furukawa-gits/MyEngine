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
	primitiveEnemy* enemy = nullptr;

	//ƒ‰ƒ“ƒ_ƒ€‚Å‰ŠúˆÊ’u‚ğŒˆ‚ß‚é
	std::random_device seed;
	std::mt19937 rnd(seed());

	std::uint32_t xResult = rnd();
	std::uint32_t yResult = rnd();
	std::uint32_t zResult = rnd();

	XMFLOAT3 startPos;

	switch (addtype)
	{
	case enemyType::tutorial:
		enemy = new TutorialEnemy;
		startPos =
		{
			(float)(xResult % 100 - 50),
			(float)(xResult % 60 + 30),
			(float)(xResult % 100 - 50)
		};
		break;

	case enemyType::chase:
		enemy = new ChaseEnemy;
		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::shot:
		enemy = new ShotEnemy;
		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::homing:
		enemy = new HomingEnemy;
		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;

	case enemyType::rampage:
		enemy = new RampageEnemy;
		startPos =
		{
			(float)(xResult % 50 - 25),
			(float)(xResult % 30 + 15),
			(float)(xResult % 50 - 25)
		};
		break;
	}

	unique_ptr<primitiveEnemy> newEnemy = std::make_unique<primitiveEnemy>();
	newEnemy.reset(enemy);
	newEnemy->init();
	newEnemy->set(startPos);

	enemyList.push_back(std::move(newEnemy));
}

void UsuallyEnemymanager::update()
{
	enemyList.remove_if([](std::unique_ptr<primitiveEnemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<primitiveEnemy>& newenemy : enemyList)
	{
		newenemy->updata();
	}
}

void UsuallyEnemymanager::draw3D()
{
	for (std::unique_ptr<primitiveEnemy>& newenemy : enemyList)
	{
		newenemy->draw3D();
	}
}

void UsuallyEnemymanager::draw2D()
{
	for (std::unique_ptr<primitiveEnemy>& newenemy : enemyList)
	{
		newenemy->draw2D();
	}
}

void UsuallyEnemymanager::drawUIs()
{
	for (std::unique_ptr<primitiveEnemy>& newenemy : enemyList)
	{
		newenemy->drawMiniMapIcon();
	}
}
