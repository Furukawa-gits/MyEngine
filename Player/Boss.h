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
	void bossDraw3D(directX* directx);
	void bossDraw2D(directX* directx);
	void bossSet(XMFLOAT3 pos);
	void bossReSet();
	void CheckHitPlayer(Player* player);

private:

};