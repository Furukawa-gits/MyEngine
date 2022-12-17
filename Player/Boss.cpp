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

	update(player->playerObject->getPosition());

	if (!Isarive && isDraw)
	{
		player->isStop = true;

		XMFLOAT3 setvec =
		{
			position.x - 5,
			position.y,
			position.z - 5,
		};

		bossCamera->SetEye(setvec);
		bossCamera->SetTarget(position);

		Object3d_FBX::SetCamera(bossCamera);
	}
}

void Boss::bossSet(XMFLOAT3 pos)
{
	set(pos);
	HP = resetHitPoint;
	arrivalTime = 300;
	bossRotEase.set(easingType::easeOut, easingPattern::Quadratic, arrivalTime, 1000, 0);
	bossScale = { 0,0,0 };
	enemyObject->SetScale(bossScale);
	enemyObject->SetPosition(pos);
	enemyObject->Update();

	//演出用カメラをセット
	bossCamera = new Camera;
	bossCamera->SetEye({ pos.x - 10,pos.y - 10,pos.z + 10 });
	bossCamera->SetTarget({ pos.x + 5,pos.y + 5,pos.z - 5 });
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

	float rot = bossRotEase.easing();

	bossScale.x += bossbaseScale.x / arrivalTime;
	bossScale.y += bossbaseScale.y / arrivalTime;
	bossScale.z += bossbaseScale.z / arrivalTime;

	XMMATRIX matrot = XMMatrixIdentity();
	matrot *= XMMatrixRotationZ(XMConvertToRadians(rot));
	matrot *= XMMatrixRotationX(XMConvertToRadians(rot));
	matrot *= XMMatrixRotationY(XMConvertToRadians(rot));

	enemyObject->setRotMatrix(matrot);
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