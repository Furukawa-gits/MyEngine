#include"Enemy.h"
#include<random>

Model* Enemy::enemyModelS = nullptr;

#pragma region “G–{‘Ì
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(enemyObject);
}

void Enemy::staticInit()
{
	enemyModelS = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	enemyBullet::staticInit();
}

void Enemy::init(enemyPattern pattern)
{
	Isarive = false;
	isDraw = false;

	rockTarget.anchorpoint = { 0.5f,0.5f };
	rockTarget.size = { 70,70 };
	rockTarget.GenerateSprite("Rock_on.png");

	enemyObject = new Object3d_FBX;
	enemyObject->Initialize();
	enemyObject->SetModel(enemyModelS);
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });

	enemyCollision.radius = 2.0f;

	enemyMovePattern = pattern;

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet.init();
		//shotCount = rand() % 10;
	}
}

void Enemy::set(XMFLOAT3 pos)
{
	position = pos;
	startPosition = pos;
	enemyObject->SetPosition(pos);
	Isarive = true;
	isTargetSet = false;
	chaseCount = 0;
	waitCount = 0;
	isChase = false;
	isWait = true;
	isDraw = true;
	if (enemyMovePattern == enemyPattern::chase)
	{
		waitCount = rand() % 40;
		isChase = false;
		isWait = true;
	}
}

void Enemy::reSet()
{
	enemyObject->SetPosition(startPosition);
	position = startPosition;
	chaseCount = 0;
	waitCount = 0;
	isChase = false;
	isWait = true;
	Isarive = true;
	HP = 1;
	isTargetSet = false;
	isSetMissile = false;
	isDraw = true;
}

void Enemy::chase(XMFLOAT3 pPos)
{
	if (enemyMovePattern != enemyPattern::chase)
	{
		return;
	}

	//’ÇÕ
	if (isChase)
	{
		//’Ç”öƒJƒEƒ“ƒg‰ÁZ
		chaseCount++;
		enemySpeed = 0.25f;
		if (chaseCount >= 1)
		{
			isChase = false;
			chaseCount = 0;
			isWait = true;
		}
	}

	//‘Ò‹@
	if (isWait)
	{
		//‘Ò‹@ƒJƒEƒ“ƒg‰ÁZ
		waitCount++;
		if (enemySpeed > 0.0f)
		{
			enemySpeed -= 0.01f;
		}
		if (waitCount >= 40)
		{
			isWait = false;
			waitCount = 0;
			isChase = true;
		}
	}

	position = enemyObject->getPosition();
	XMFLOAT3 dis = { pPos.x - position.x,pPos.y - position.y,pPos.z - position.z };

	float lengthDis = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	dis.x /= lengthDis;
	dis.y /= lengthDis;
	dis.z /= lengthDis;

	position.x += dis.x * enemySpeed;
	position.y += dis.y * enemySpeed;
	position.z += dis.z * enemySpeed;
}

void Enemy::shot(XMFLOAT3 pPos)
{
	if (enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	XMFLOAT3 startToTarget =
	{
		pPos.x - this->position.x,
		pPos.y - this->position.y,
		pPos.z - this->position.z
	};

	float length = sqrtf(powf(startToTarget.x, 2) + powf(startToTarget.y, 2) + powf(startToTarget.z, 2));

	if (length <= 40)
	{
		isInRange = true;
	}
	else
	{
		isInRange = false;
	}

	if (!bullet.isBulletArive() && isInRange)
	{
		shotCount++;
	}

	if (shotCount >= 10 && bullet.isBulletArive() == false)
	{
		isShot = true;
		shotCount = 0;
	}
	else
	{
		isShot = false;
	}

	if (isShot)
	{
		bullet.set(pPos, this->position);
		isShot = false;
	}

	bullet.update();
}

void Enemy::update(XMFLOAT3 playerPos)
{
	if (!isDraw)
	{
		return;
	}

	enemyObject->Update();

	if (isStop)
	{
		return;
	}

	//“G‚ª¶‘¶
	ariveMove(playerPos);

	//“G‚ªŒ‚’Ä
	deathMove();

	return;
}

void Enemy::ariveMove(XMFLOAT3 playerPos)
{
	if (!Isarive)
	{
		return;
	}

	chase(playerPos);

	shot(playerPos);

	//HP‚ª0‚É‚È‚Á‚½‚çÁ–Å
	if (HP <= 0)
	{
		Isarive = false;
		fallDownCount = 0;
	}

	if (isTargetSet)
	{
		rockTarget.rotation += 1.5f;
		XMFLOAT2 screenPos = enemyObject->worldToScleen();
		rockTarget.position = { screenPos.x,screenPos.y,0 };
	}

	rockTarget.SpriteTransferVertexBuffer();
	rockTarget.SpriteUpdate();

	enemyObject->SetPosition(position);
	enemyCollision.center =
	{
		enemyObject->getPosition().x,
		enemyObject->getPosition().y,
		enemyObject->getPosition().z,1.0f
	};
}

void Enemy::deathMove()
{
	if (Isarive)
	{
		return;
	}

	fallDownCount++;

	position.y -= 0.2f;
	rot.x += deathRotSpeed;
	rot.y += deathRotSpeed;
	rot.z += deathRotSpeed;
	enemyObject->setRotMatrix(rot.x, rot.y, rot.z);

	if (fallDownCount >= 90)
	{
		isDraw = false;
		fallDownCount = 0;
	}

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet.isArive = false;
	}
}

void Enemy::isHitTarget(XMFLOAT2 targetpos, bool istarget)
{
	if (!Isarive)
	{
		return;
	}

	XMFLOAT2 screenPos = enemyObject->worldToScleen();

	float dis = sqrtf(powf(targetpos.x - screenPos.x, 2) + powf(targetpos.y - screenPos.y, 2));

	if (dis < 20 && istarget && !isTargetSet)
	{
		isTargetSet = true;
	}

	rockTarget.position = { screenPos.x,screenPos.y,0 };
}

void Enemy::isHitShot(XMFLOAT2 targetpos)
{
	if (!Isarive)
	{
		return;
	}

	XMFLOAT2 screenPos = enemyObject->worldToScleen();

	float dis = sqrtf(powf(targetpos.x - screenPos.x, 2) + powf(targetpos.y - screenPos.y, 2));

	if (dis < 30)
	{
		HP--;

		if (HP <= 0)
		{
			Isarive = false;
		}
	}
}

void Enemy::draw3D(directX* directx)
{
	if (!isDraw)
	{
		return;
	}

	enemyObject->SetPipelineSimple(directx->cmdList.Get());
	enemyObject->Draw(directx->cmdList.Get());

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet.draw(directx);
	}
}

void Enemy::draw2D(directX* directx)
{
	if (Isarive && isTargetSet)
	{
		rockTarget.DrawSprite(directx->cmdList.Get());
	}
}
#pragma endregion

#pragma region “G‚Ì’e
Model* enemyBullet::buletModelS = nullptr;

enemyBullet::enemyBullet()
{
}

enemyBullet::~enemyBullet()
{
}

void enemyBullet::staticInit()
{
	buletModelS = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");
}

void enemyBullet::init()
{
	bulletObject = new Object3d_FBX;
	bulletObject->Initialize();
	bulletObject->SetModel(buletModelS);
	bulletObject->SetScale({ 0.3f,0.3f,0.3f });

	bulletObject->setColor({ 0,0,1,1 });

	bulletCollision.radius = 0.3f;
}

void enemyBullet::set(XMFLOAT3 playerpos, XMFLOAT3 shotpos)
{
	position = shotpos;

	XMFLOAT3 startToTarget =
	{
		playerpos.x - shotpos.x,
		playerpos.y - shotpos.y,
		playerpos.z - shotpos.z
	};

	float length = sqrtf(powf(startToTarget.x, 2) + powf(startToTarget.y, 2) + powf(startToTarget.z, 2));

	bulletVec =
	{
		startToTarget.x / length,
		startToTarget.y / length,
		startToTarget.z / length
	};

	ariveTime = 0;
	isArive = true;
}

void enemyBullet::update()
{
	if (!isArive)
	{
		return;
	}

	ariveTime++;

	position.x += bulletVec.x * bulletSpeed;
	position.y += bulletVec.y * bulletSpeed;
	position.z += bulletVec.z * bulletSpeed;

	rot.x += 4;
	rot.y += 4;
	rot.z += 4;
	bulletObject->setRotMatrix(rot.x, rot.y, rot.z);

	bulletObject->SetPosition(position);
	bulletObject->Update();

	bulletCollision.center =
	{
		bulletObject->getPosition().x,
		bulletObject->getPosition().y,
		bulletObject->getPosition().z,1.0f
	};

	if (ariveTime >= 100)
	{
		isArive = false;
		ariveTime = 0;
	}
}

void enemyBullet::draw(directX* directx)
{
	if (!isArive)
	{
		return;
	}

	bulletObject->SetPipelineSimple(directx->cmdList.Get());
	bulletObject->Draw(directx->cmdList.Get());
}
#pragma endregion