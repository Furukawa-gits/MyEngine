#pragma once
#include"Bullets.h"

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	void init(dxinput* input);

	void updata();

	void addBullet(XMFLOAT3 start, XMFLOAT3 target);

	void addMissile(Enemy* enemy, int& targetnum, XMFLOAT3 start);

	void resetParam();

	void draw(directX* directx);

	std::list<std::unique_ptr<bullet>>* getBullet() { return &bulletsList; }
	std::list<std::unique_ptr<Missile>>* getMissile() { return &missilesList; }

public:
	std::list<std::unique_ptr<bullet>> bulletsList;
	std::list<std::unique_ptr<Missile>> missilesList;

private:
	static dxinput* input;

	const int maxMissileNum = 8;

	const int nextMissileTime = 15;

	int waitTime = 0;
};