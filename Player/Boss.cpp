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
	bossbaseScale = { 5,5,5 };
	enemyObject->setColor({ 0.3f,0.3f,0.3f,1 });
	enemyCollision.radius = 9.0f;
	deathRotSpeed = 0.1f;
	isThisBoss = true;
}

void Boss::bossUpdate(Player* player)
{
	bossArrival(player);

	if (!isAppear)
	{
		update(player->playerObject->getPosition());
	}
}

void Boss::bossSet(XMFLOAT3 pos)
{
	set(pos);
	HP = resetHitPoint;
	arrivalTime = 600;
	bossRotEase.set(easingType::easeInOut, easingPattern::Quadratic, arrivalTime, 360, 0);
	bossScale = { 0,0,0 };
	enemyObject->SetScale(bossScale);
	enemyObject->SetPosition(pos);
	enemyObject->Update();

	//演出用カメラをセット
	bossCamera = new Camera;
	bossCamera->SetEye({ pos.x - 1,pos.y,pos.z + 1 });
	bossCamera->SetTarget(pos);
	Object3d_FBX::SetCamera(bossCamera);

	isAppear = true;
}

void Boss::bossReSet()
{
	reSet();
	HP = resetHitPoint;
	isDraw = false;
}

void Boss::bossArrival(Player* player)
{
	if (!isAppear)
	{
		return;
	}

	player->isStop = true;

	isStop = true;

	XMFLOAT3 rot =
	{
		bossRotEase.easing(),
		bossRotEase.easing(),
		bossRotEase.easing()
	};

	bossScale.x += bossbaseScale.x / arrivalTime;
	bossScale.y += bossbaseScale.y / arrivalTime;
	bossScale.z += bossbaseScale.z / arrivalTime;

	enemyObject->SetRotation(rot);
	enemyObject->SetScale(bossScale);
	enemyObject->Update();

	if (!bossRotEase.getIsActive())
	{
		enemyObject->SetRotation({ 0,0,0 });
		isStop = false;
		isAppear = false;
		player->isStop = false;
		Object3d_FBX::SetCamera(player->followCamera);
	}
}