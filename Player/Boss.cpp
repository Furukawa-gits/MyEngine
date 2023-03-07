#include "Boss.h"

std::unique_ptr<Model> Boss::bossModel = std::make_unique<Model>();

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::staticInitBoss()
{
	bossModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("boss"));
}

void Boss::bossInit()
{
	isThisBoss = true;

	Isarive = false;
	isDraw = false;

	rockTarget = std::make_unique<SingleSprite>();
	rockTarget->anchorpoint = { 0.5f,0.5f };
	rockTarget->size = { 70,70 };
	rockTarget->GenerateSprite("Rock_on.png");

	outScreenIcon[0] = std::make_unique<SingleSprite>();
	outScreenIcon[0]->anchorpoint = { 0.5f,0.5f };
	outScreenIcon[0]->size = { 100,100 };
	outScreenIcon[0]->GenerateSprite("enemyPos.png");

	outScreenIcon[1] = std::make_unique<SingleSprite>();
	outScreenIcon[1]->anchorpoint = { 0.5f,0.5f };
	outScreenIcon[1]->size = { 100,100 };
	outScreenIcon[1]->GenerateSprite("!.png");

	enemyObject = new Object3d_FBX;
	enemyObject->Initialize();
	enemyObject->SetModel(bossModel.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });
	enemyObject->setColor({ 0.3f,0.3f,0.3f,1 });

	enemyCollision.radius = 2.0f;

	enemyMovePattern = enemyPattern::shot;

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet = std::make_unique<enemyBullet>();
		bullet->init();
	}

	HP = 5;
	bossbaseScale = { 5,5,5 };
	enemyCollision.radius = 9.0f;
	deathRotSpeed = 0.1f;
}

void Boss::bossUpdate(Player* player)
{
	enemyObject->Update();

	bossArrival(player);

	playerPointer = player;

	XMFLOAT3 playerPos = playerPointer->playerObject->getPosition();

	if (isStop)
	{
		return;
	}

	XMFLOAT3 toPlayer =
	{
		position.x - playerPos.x ,
		position.y - playerPos.y ,
		position.z - playerPos.z ,
	};

	float length = sqrtf(powf(toPlayer.x, 2) + powf(toPlayer.y, 2) + powf(toPlayer.z, 2));

	if (length >= forPlayer)
	{
		isFar = true;
	}
	else
	{
		isFar = false;
	}

	XMFLOAT3 playerFront = playerPointer->followCamera->getFrontVec();

	//ベクトルの内積から角度を求めて、ロックオンの範囲を絞る
	float cross = toPlayer.x * playerFront.x + toPlayer.y * playerFront.y + toPlayer.z * playerFront.z;

	float vecLen = sqrtf((powf(toPlayer.x, 2) + powf(toPlayer.y, 2) + powf(toPlayer.z, 2)) * (powf(playerFront.x, 2) + powf(playerFront.y, 2) + powf(playerFront.z, 2)));

	toPlayerAngle = acosf(cross / vecLen) * (180.0f / (float)M_PI);

	if (toPlayerAngle > 90)
	{
		//画面外(自分の位置がプレイヤーの向きと直角以上)にいるとき画面外フラグを立てる
		isOutScreen = true;
	}
	else
	{
		isOutScreen = false;
	}

	bossAriveMove();

	bossDeathMove();
}

void Boss::bossSpriteUpdata()
{
	XMFLOAT2 targetPos = enemyObject->worldToScleen();
	XMFLOAT2 targetPosOutScreen = enemyObject->worldToScleen();

	rockTarget->rotation += 1.5f;

	//ターゲットアイコン
	if (targetPos.x < rockTarget->size.x / 2)
	{
		targetPos.x = rockTarget->size.x / 2;
	}
	else if (targetPos.x > 1280 - rockTarget->size.x / 2)
	{
		targetPos.x = 1280 - rockTarget->size.x / 2;
	}
	if (targetPos.y < rockTarget->size.y / 2)
	{
		targetPos.y = rockTarget->size.y / 2;
	}
	else if (targetPos.y > 720 - rockTarget->size.y / 2)
	{
		targetPos.y = 720 - rockTarget->size.y / 2;
	}

	//画面外アイコン
	if (targetPosOutScreen.x < outScreenIcon[0]->size.x / 2)
	{
		targetPosOutScreen.x = outScreenIcon[0]->size.x / 2;
	}
	else if (targetPosOutScreen.x > 1280 - outScreenIcon[0]->size.x / 2)
	{
		targetPosOutScreen.x = 1280 - outScreenIcon[0]->size.x / 2;
	}
	if (targetPosOutScreen.y < outScreenIcon[0]->size.y / 2)
	{
		targetPosOutScreen.y = outScreenIcon[0]->size.y / 2;
	}
	else if (targetPosOutScreen.y > 720 - outScreenIcon[0]->size.y / 2)
	{
		targetPosOutScreen.y = 720 - outScreenIcon[0]->size.y / 2;
	}


	if (isOutScreen)
	{
		targetPos.y = 720 - rockTarget->size.y / 2;
		targetPosOutScreen.y = 720 - outScreenIcon[0]->size.y / 2;
	}

	rockTarget->position = { targetPos.x,targetPos.y,0 };
	outScreenIcon[0]->position = { targetPosOutScreen.x,targetPosOutScreen.y,0 };
	outScreenIcon[1]->position = { targetPosOutScreen.x,targetPosOutScreen.y,0 };

	rockTarget->SpriteTransferVertexBuffer();
	rockTarget->SpriteUpdate();

	outScreenIcon[0]->SpriteTransferVertexBuffer();
	outScreenIcon[0]->SpriteUpdate();
	outScreenIcon[1]->SpriteTransferVertexBuffer();
	outScreenIcon[1]->SpriteUpdate();
}

void Boss::bossSet(XMFLOAT3 pos)
{
	position = pos;
	startPosition = pos;
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

	nextBulletTime = 0;
	bulletCount = 0;
	maxBulletCount = 10;
	isRampageWait = true;
	rampageWaitCount = 0;
	Bullets.clear();

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
	player->isInvisible = 1;

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
		enemyObject->SetPosition(position);
		enemyObject->Update();
		isStop = false;
		Isarive = true;
		isAppear = false;
		player->isStop = false;
		player->isInvisible = -1;
		Object3d_FBX::SetCamera(player->followCamera);
	}
}

void Boss::bossAriveMove()
{
	if (!Isarive || isAppear)
	{
		return;
	}

	bossChase();

	bossShot();

	bossHoming();

	//HPが0になったら消滅
	if (HP <= 0)
	{
		Isarive = false;
		fallDownCount = 0;
		deathRotSpeed = 0.5f;
	}

	bossSpriteUpdata();

	enemyObject->SetPosition(position);
	enemyCollision.center =
	{
		enemyObject->getPosition().x,
		enemyObject->getPosition().y,
		enemyObject->getPosition().z,1.0f
	};
}

void Boss::bossDeathMove()
{
	if (Isarive || isAppear)
	{
		return;
	}

	playerPointer->isStop = true;
	playerPointer->isInvisible = 1;

	position = enemyObject->getPosition();

	XMFLOAT3 setvec =
	{
		position.x - 35,
		position.y,
		position.z,
	};

	bossCamera->SetEye(setvec);
	bossCamera->SetTarget(enemyObject->getPosition());
	bossCamera->Update();

	Object3d_FBX::SetCamera(bossCamera);
	SingleParticle::setCamera(bossCamera);

	//撃墜演出のカウント
	fallDownCount++;

	float scale = 1.0f - ((float)fallDownCount / ((float)maxFallCount * 3));

	rot.x += deathRotSpeed;
	rot.y += deathRotSpeed;
	rot.z += deathRotSpeed;
	enemyObject->setRotMatrix(rot.x, rot.y, rot.z);
	enemyObject->SetScale({ bossScale.x * scale,bossScale.y * scale,bossScale.z * scale });

	//一定間隔でエフェクト
	if (fallDownCount % 20 == 0)
	{
#pragma region パーティクル生成
		//生成位置を球体表面上で指定
		float radius = 10;
		float theta = (float)(rand() % 180) - 90.0f;
		float phi = (float)(rand() % 360);
		XMFLOAT3 spherepos =
		{
			radius * sinf(phi) * cosf(theta),
			radius * sinf(phi) * sinf(theta),
			radius * sinf(phi),
		};
		XMFLOAT3 startPos =
		{
			enemyObject->getPosition().x + spherepos.x,
			enemyObject->getPosition().y + spherepos.y,
			enemyObject->getPosition().z + spherepos.z
		};

		//爆発パーティクル
		std::unique_ptr<SingleParticle> newBomparticle = std::make_unique<SingleParticle>();
		newBomparticle->generate();
		newBomparticle->set(50, startPos, { 0,0,0 }, { 0,0,0 }, 0.2, 9.0);
		bomParticles.push_back(std::move(newBomparticle));

		//黒煙パーティクル
		std::unique_ptr<SingleParticle> newSmokeparticle = std::make_unique<SingleParticle>();
		newSmokeparticle->generate();
		newSmokeparticle->set(50, startPos, { 0,0,0 }, { 0,0,0 }, 0.2, 5.0);
		smokeParticles.push_back(std::move(newSmokeparticle));
#pragma endregion パーティクル生成
	}

	//爆発パーティクル更新
	bomParticles.remove_if([](std::unique_ptr<SingleParticle>& newparticle)
		{
			return newparticle->frame == newparticle->num_frame;
		});
	for (std::unique_ptr<SingleParticle>& newparticle : bomParticles)
	{
		newparticle->updata();
	}

	//煙パーティクル更新
	smokeParticles.remove_if([](std::unique_ptr<SingleParticle>& newparticle)
		{
			return newparticle->frame == newparticle->num_frame;
		});
	for (std::unique_ptr<SingleParticle>& newparticle : smokeParticles)
	{
		newparticle->updata();
	}

	//落ちきったら
	if (fallDownCount >= maxFallCount * 3)
	{
		bomParticles.clear();
		smokeParticles.clear();
		isDraw = false;
		fallDownCount = 0;
	}

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet->isArive = false;
	}
}

void Boss::bossChase()
{
	if (enemyMovePattern != enemyPattern::chase)
	{
		return;
	}

	if (isAppear)
	{
		return;
	}

	if (!playerPointer->isArive)
	{
		return;
	}

	//追跡
	if (isChase)
	{
		//追尾カウント加算
		chaseCount++;
		enemySpeed = 0.35f;
		if (chaseCount >= 1)
		{
			isChase = false;
			chaseCount = 0;
			isWait = true;
		}
	}

	//待機
	if (isWait)
	{
		//待機カウント加算
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

	XMFLOAT3 pPos = playerPointer->playerObject->getPosition();

	position = enemyObject->getPosition();
	XMFLOAT3 dis = { pPos.x - position.x,pPos.y - position.y,pPos.z - position.z };

	float lengthDis = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	dis.x /= lengthDis;
	dis.y /= lengthDis;
	dis.z /= lengthDis;

	position.x += dis.x * enemySpeed;
	position.y += dis.y * enemySpeed;
	position.z += dis.z * enemySpeed;

	enemyObject->SetPosition(position);
}

void Boss::bossShot()
{
	if (enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	if (isAppear)
	{
		return;
	}

	if (!playerPointer->isArive)
	{
		return;
	}

	XMFLOAT3 pPos = playerPointer->playerObject->getPosition();

	XMFLOAT3 startToTarget =
	{
		pPos.x - this->position.x,
		pPos.y - this->position.y,
		pPos.z - this->position.z
	};

	float length = sqrtf(powf(startToTarget.x, 2) + powf(startToTarget.y, 2) + powf(startToTarget.z, 2));

	if (length <= forPlayer / 2)
	{
		isInRange = true;
	}
	else
	{
		isInRange = false;
	}

	if (!bullet->isBulletArive() && isInRange)
	{
		nextShotTime++;
	}

	if (nextShotTime >= 10 && bullet->isBulletArive() == false)
	{
		isShot = true;
		nextShotTime = 0;
	}
	else
	{
		isShot = false;
	}

	if (isShot)
	{
		bullet->set(pPos, this->position);
		isShot = false;
	}

	bullet->update();
}

void Boss::bossHoming()
{
	if (enemyMovePattern != enemyPattern::homing)
	{
		return;
	}

	if (isAppear)
	{
		return;
	}

	if (!playerPointer->isArive)
	{
		return;
	}

	XMFLOAT3 pPos = playerPointer->playerObject->getPosition();

	XMFLOAT3 startToTarget =
	{
		pPos.x - this->position.x,
		pPos.y - this->position.y,
		pPos.z - this->position.z
	};

	float length = sqrtf(powf(startToTarget.x, 2) + powf(startToTarget.y, 2) + powf(startToTarget.z, 2));

	if (length <= forPlayer / 2)
	{
		isInRange = true;
	}
	else
	{
		isInRange = false;
	}

	if (!bullet->isBulletArive() && isInRange)
	{
		nextShotTime++;
	}

	if (nextShotTime >= 10 && bullet->isBulletArive() == false)
	{
		isShot = true;
		nextShotTime = 0;
	}
	else
	{
		isShot = false;
	}

	if (isShot)
	{
		bullet->set(pPos, this->position);
		isShot = false;
	}

	bullet->update();

	//追跡
	if (isChase)
	{
		//追尾カウント加算
		chaseCount++;
		enemySpeed = 0.35f;
		if (chaseCount >= 1)
		{
			isChase = false;
			chaseCount = 0;
			isWait = true;
		}
	}

	//待機
	if (isWait)
	{
		//待機カウント加算
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

	enemyObject->SetPosition(position);
}

void Boss::bossRampage()
{
	if (enemyMovePattern != enemyPattern::rampage)
	{
		return;
	}

	if (isAppear)
	{
		return;
	}

	if (!playerPointer->isArive)
	{
		return;
	}

	Bullets.remove_if([](std::unique_ptr<enemyBullet>& bullet)
		{
			return bullet->isBulletArive() == false;
		});

	for (std::unique_ptr<enemyBullet>& bullet : Bullets)
	{
		bullet->update();
	}

	if (isRampageWait)
	{
		rampageWaitCount++;

		if (rampageWaitCount >= 100)
		{
			isRampageWait = false;
		}
		return;
	}

	nextBulletTime++;

	if (nextBulletTime % 15 == 0)
	{
		std::unique_ptr<enemyBullet> newBullet = std::make_unique<enemyBullet>();
		newBullet->init();

		XMFLOAT3 rampageTargetPos =
		{
			playerPointer->getPlayerPos().x - (float)(rand() % 8 - 4),
			playerPointer->getPlayerPos().y - (float)(rand() % 8 - 4),
			playerPointer->getPlayerPos().z - (float)(rand() % 8 - 4)
		};

		newBullet->set(rampageTargetPos, this->position);
		Bullets.push_back(std::move(newBullet));

		bulletCount++;
	}

	if (bulletCount == maxBulletCount)
	{
		isRampageWait = true;
		rampageWaitCount = 0;
		nextBulletTime = 0;
		bulletCount = 0;
	}
}
