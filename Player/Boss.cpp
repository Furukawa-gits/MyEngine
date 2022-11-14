#include "Boss.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::bossInit()
{
	init(enemyPattern::shot);
	HP = 5;
	enemyObject->SetScale({ 5.0f,5.0f,5.0f });
	enemyObject->setColor({ 0.3f,0.3f,0.3f,1 });
	enemyCollision.radius = 9.0f;
	deathRotSpeed = 0.1f;
	isThisBoss = true;
}

void Boss::bossUpdate(Player* player)
{
	update(player->playerObject->getPosition());
}

void Boss::bossSet(XMFLOAT3 pos)
{
	set(pos);
}

void Boss::bossReSet()
{
	reSet();
	HP = 5;
	isDraw = false;
}

void Boss::CheckHitPlayer(Player* player)
{
	
}
