#pragma once
#include"Player.h"

class Boss
	:public Enemy
{
public:
	Boss();
	~Boss();

	void bossInit();
	void bossUpdate(Player* player);
	void bossSet(XMFLOAT3 pos);
	void bossReSet();
	void setHitPoint(int hp)
	{
		HP = hp;
		resetHitPoint = hp;
		return;
	}
	void bossArrival();

private:
	int resetHitPoint = 3;
};