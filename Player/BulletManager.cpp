#include "BulletManager.h"

dxinput* BulletManager::input = nullptr;

BulletManager::BulletManager()
{
	PrimitiveBullet::staticInit();
}

BulletManager::~BulletManager()
{
	normalBulletsList.clear();
	missilesList.clear();
}

void BulletManager::init(dxinput* input)
{
	this->input = input;
	normalBulletsList.clear();
	missilesList.clear();
}

void BulletManager::updata()
{
	//���񂾒e�͍폜
	normalBulletsList.remove_if([](std::unique_ptr<NormalBullet>& newbullet)
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

	//�ʏ�e�̍X�V(���j�[�N���X�g)
	for (std::unique_ptr<NormalBullet>& bullet : normalBulletsList)
	{
		bullet->updata();
	}

	//�~�T�C���̍X�V(���j�[�N���X�g)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->updata();
	}
}

void BulletManager::addBullet(XMFLOAT3 start, XMFLOAT3 target, int& shotcount, bool& shotflag)
{
	if (!input->Mouse_LeftTriger() || !shotflag)
	{
		return;
	}

	//���X�g��
	std::unique_ptr<NormalBullet> newBullet = std::make_unique<NormalBullet>();
	newBullet->init({ 0,1,0,1 }, { 0,1,0,1 });
	newBullet->set(start, target);

	normalBulletsList.push_back(std::move(newBullet));

	shotcount++;
}

void BulletManager::addMissile(Enemy* enemy, int& targetnum, XMFLOAT3 start)
{
	//�~�T�C���ǉ�
	std::unique_ptr<Missile> newMissile = std::make_unique<Missile>();
	newMissile->init({ 1,1,0,1 }, { 1,1,0,1 });
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
	//�ʏ�e�̕`��(���j�[�N���X�g)
	for (std::unique_ptr<NormalBullet>& bullet : normalBulletsList)
	{
		bullet->draw(directx);
	}
	//�~�T�C���̕`��(���j�[�N���X�g)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->draw(directx);
	}
}
