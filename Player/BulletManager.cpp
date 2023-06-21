#include "BulletManager.h"

dxinput* BulletManager::input = nullptr;

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
	bulletsList.clear();
	missilesList.clear();
}

void BulletManager::init(dxinput* input)
{
	this->input = input;
	bulletsList.clear();
	missilesList.clear();
}

void BulletManager::updata()
{
	//死んだ弾は削除
	bulletsList.remove_if([](std::unique_ptr<bullet>& newbullet)
		{
			return newbullet->isAlive == false;
		});

	missilesList.remove_if([](std::unique_ptr<Missile>& newmissile)
		{
			return newmissile->isAlive == false;
		});

	missilesList.remove_if([](std::unique_ptr<Missile>& newmissile)
		{
			return newmissile->enemyPointer == nullptr;
		});

	//通常弾の更新(ユニークリスト)
	for (std::unique_ptr<bullet>& bullet : bulletsList)
	{
		bullet->update();
	}

	//ミサイルの更新(ユニークリスト)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->update();
	}
}

void BulletManager::addBullet(XMFLOAT3 start, XMFLOAT3 target, int* shotcount, bool& shotflag)
{
	if (!input->Mouse_LeftTriger() || !shotflag)
	{
		return;
	}

	//リスト化
	std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
	newBullet->init();
	newBullet->set(start, target);

	bulletsList.push_back(std::move(newBullet));

	shotcount++;
}

void BulletManager::addMissile(Enemy* enemy, int& targetnum, XMFLOAT3 start)
{
	//ミサイル追加
	std::unique_ptr<Missile> newMissile = std::make_unique<Missile>();
	newMissile->init();
	newMissile->setPenemy(enemy);
	newMissile->start(start);
	missilesList.push_back(std::move(newMissile));
}

void BulletManager::resetParam()
{
	waitTime = nextMissileTime;
}

void BulletManager::draw(directX* directx)
{
	//通常弾の描画(ユニークリスト)
	for (std::unique_ptr<bullet>& bullet : bulletsList)
	{
		bullet->draw(directx);
	}
	//ミサイルの描画(ユニークリスト)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->draw(directx);
	}
}
