#include "Boss.h"

void Boss::bossInit()
{
	init(enemyPattern::chase);
	HP = 30;
	enemyObject->SetScale({ 5.0f,5.0f,5.0f });
	enemyObject->setColor({ 0.3f,0.3f,0.3f,1 });
	enemyCollision.radius = 9.0f;
}

void Boss::bossUpdate(Player* player)
{
	update(player->playerObject->getPosition());
}

void Boss::bossDraw3D(directX* directx)
{
	draw3D(directx);
}

void Boss::bossDraw2D(directX* directx)
{
	draw2D(directx);
}

void Boss::bossSet(XMFLOAT3 pos)
{
	set(pos);
}

void Boss::bossReSet()
{
	reSet();
}

void Boss::CheckHitPlayer(Player* player)
{
	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{

	}
}
