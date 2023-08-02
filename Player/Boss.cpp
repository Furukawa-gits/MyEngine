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

	isAlive = false;
	isDraw = false;

	rockTarget = std::make_unique<SingleSprite>();
	rockTarget->anchorpoint = { 0.5f,0.5f };
	rockTarget->size = { 70,70 };
	rockTarget->generateSprite("Rock_on.png");

	outScreenIcon[0] = std::make_unique<SingleSprite>();
	outScreenIcon[0]->anchorpoint = { 0.5f,0.5f };
	outScreenIcon[0]->size = { 100,100 };
	outScreenIcon[0]->generateSprite("enemyPos.png");

	outScreenIcon[1] = std::make_unique<SingleSprite>();
	outScreenIcon[1]->anchorpoint = { 0.5f,0.5f };
	outScreenIcon[1]->size = { 100,100 };
	outScreenIcon[1]->generateSprite("!.png");

	bossHitPointGauge.anchorpoint = { 0.5f,0.5f };
	bossHitPointGauge.size = { 50,20 };
	bossHitPointGauge.position = { 640,40,0 };
	bossHitPointGauge.generateSprite("bossHPGauge.png");

	miniMapEnemy.anchorpoint = { 0.5f,0.5f };
	miniMapEnemy.size = { 15,15 };
	miniMapEnemy.generateSprite("enemyIcon.png");

	enemyHeight.anchorpoint = { 0.5f,0.5f };
	enemyHeight.size = { 32,3 };
	enemyHeight.generateSprite("bossHPGauge.png");

	enemyObject = new object3dFBX;
	enemyObject->initialize();
	enemyObject->SetModel(bossModel.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });
	enemyObject->setColor({ 0.8f,0.8f,0.8f,1 });

	enemyMovePattern = enemyPattern::shot;

	HP = 5;
	bossbaseScale = { 5,5,5 };
	enemyCollision.radius = 9.0f;
	deathRotSpeed = 0.1f;
}

void Boss::bossUpdate(Player* player)
{
	enemyObject->updata();

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

	//ミニマップアイコン更新
	XMFLOAT3 bossPosition = position;

	//上から見た位置なので X・Z座標
	XMFLOAT3 minimapPosition =
	{
		(bossPosition.x * 0.21f) + Enemy::miniMapPosition.x,
		(bossPosition.z * 0.21f) + Enemy::miniMapPosition.x,
		0
	};

	miniMapEnemy.position = minimapPosition;
	miniMapEnemy.spriteUpdata();

	bossAriveMove();

	bossDeathMove();
}

void Boss::bossSpriteUpdata()
{
	XMFLOAT2 targetPos = enemyObject->worldToScleen();
	XMFLOAT2 targetPosOutScreen = enemyObject->worldToScleen();

	rockTarget->rotation += 2.5f;

	//ロックオンアイコンのサイズが大きければデフォルトに近づけていく
	if (rockTarget->size.x > defaultRockIconSize.x)
	{
		rockTarget->size.x -= decreaseSize;
		rockTarget->size.y -= decreaseSize;
	}
	else
	{
		rockTarget->size = defaultRockIconSize;
	}

	//各アイコンが画面外に行かないようにする
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

	//プレイヤーの視界外に行ったら、画面の下にアイコンを移動させる
	if (isOutScreen)
	{
		targetPos.y = 720 - rockTarget->size.y / 2;
		targetPosOutScreen.y = 720 - outScreenIcon[0]->size.y / 2;
	}

	rockTarget->position = { targetPos.x,targetPos.y,0 };
	outScreenIcon[0]->position = { targetPosOutScreen.x,targetPosOutScreen.y,0 };
	outScreenIcon[1]->position = { targetPosOutScreen.x,targetPosOutScreen.y,0 };

	rockTarget->spriteUpdata();

	outScreenIcon[0]->spriteUpdata();
	outScreenIcon[1]->spriteUpdata();

	bossHitPointGauge.size.x = (float)HP * 50;
	bossHitPointGauge.spriteUpdata();

	//高度メーター更新
	//y座標に補正をかけてpositionに代入
	XMFLOAT3 position = enemyObject->getPosition();
	float enemyH = position.y - -180;

	//スプライトの座標系に変更＆メーターの上がり具合を調整
	enemyH = -(enemyH * 0.3333f);

	//実際に表示する座標に変更
	enemyH = enemyH + miniMapPosition.y + 100;

	float underLimit = miniMapPosition.y + 100 - 5;
	float upperLimit = miniMapPosition.y - 100 + 5;

	if (enemyH < upperLimit)
	{
		enemyH = upperLimit;
	}
	else if (enemyH > underLimit)
	{
		enemyH = underLimit;
	}

	enemyHeight.position = { miniMapPosition.x + 120,enemyH,0 };
	miniMapEnemy.position = { miniMapPosition.x + 140,enemyH - 2,0 };

	miniMapEnemy.spriteUpdata();
	enemyHeight.spriteUpdata();
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
	normalBullets.clear();

	HP = resetHitPoint;
	arrivalTime = 300;
	bossRotEase.set(easingType::easeOut, easingPattern::Quadratic, arrivalTime, 1000, 0);
	bossScale = { 0,0,0 };
	enemyObject->SetScale(bossScale);
	enemyObject->SetPosition(pos);
	enemyObject->updata();

	//演出用カメラをセット
	bossCamera = new Camera;
	bossCamera->SetEye({ pos.x - 10,pos.y - 10,pos.z + 10 });
	bossCamera->SetTarget({ pos.x + 5,pos.y + 5,pos.z - 5 });
	object3dFBX::SetCamera(bossCamera);

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
	enemyObject->updata();

	if (!bossRotEase.getIsActive())
	{
		enemyObject->SetRotation({ 0,0,0 });
		enemyObject->SetPosition(position);
		enemyObject->updata();
		isStop = false;
		isAlive = true;
		isAppear = false;
		player->isStop = false;
		player->isInvisible = -1;
		object3dFBX::SetCamera(player->followCamera);
	}
}

void Boss::bossAriveMove()
{
	if (!isAlive || isAppear)
	{
		return;
	}

	bossChase();

	bossShot();

	bossHoming();

	//HPが0になったら消滅
	if (HP <= 0)
	{
		isAlive = false;
		fallDownCount = 0;
		deathRotSpeed = 0.5f;

		//弾も消す
		normalBullets.clear();
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
	if (isAlive || isAppear)
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

	object3dFBX::SetCamera(bossCamera);
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
		//生成位置をランダムで指定
		float randX = (float)(rand() % 40) - 20;
		float randY = (float)(rand() % 40) - 20;
		float randZ = (float)(rand() % 40) - 20;
		XMFLOAT3 startPos =
		{
			enemyObject->getPosition().x + randX,
			enemyObject->getPosition().y + randY,
			enemyObject->getPosition().z + randZ
		};

		//爆発パーティクル
		SingleParticle newbp;
		newbp.generate();
		newbp.set(50, startPos, { 0,0,0 }, { 0,0,0 }, 0.2, 9.0);
		particleManagerOnTime::addParticle(newbp, "bomb.png");

		//黒煙パーティクル
		SingleParticle newsp;
		newsp.generate();
		newsp.set(50, startPos, { 0,0,0 }, { 0,0,0 }, 0.2, 5.0);
		particleManagerOnTime::addParticle(newsp, "smoke.png");
#pragma endregion パーティクル生成
	}

	//落ちきったら
	if (fallDownCount >= maxFallCount * 3)
	{
		isDraw = false;
		fallDownCount = 0;
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

	if (!Enemy::playerIsAlive)
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

	if (!Enemy::playerIsAlive)
	{
		return;
	}

	normalBullets.remove_if([](std::unique_ptr<NormalBullet>& bullet)
		{
			return bullet->isAlive == false;
		});

	for (std::unique_ptr<NormalBullet>& bullet : normalBullets)
	{
		bullet->updata();
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

	if (isInRange)
	{
		nextShotTime++;
	}

	if (nextShotTime >= 80)
	{
		isShot = true;
	}

	if (isShot)
	{
		std::unique_ptr<NormalBullet> newBullet = std::make_unique<NormalBullet>();
		newBullet->init({ 1,0,0,1 }, { 1,0,0,1 });

		XMFLOAT3 rampageTargetPos =
		{
			playerPosition.x - (float)(rand() % 8 - 4),
			playerPosition.y - (float)(rand() % 8 - 4),
			playerPosition.z - (float)(rand() % 8 - 4)
		};

		newBullet->bulletSpeed = 0.9f;
		newBullet->set(this->position, rampageTargetPos);
		normalBullets.push_back(std::move(newBullet));

		nextShotTime = 0;
		isShot = false;
	}
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

	if (!Enemy::playerIsAlive)
	{
		return;
	}

	//弾の更新
	normalBullets.remove_if([](std::unique_ptr<NormalBullet>& bullet)
		{
			return bullet->isAlive == false;
		});

	for (std::unique_ptr<NormalBullet>& bullet : normalBullets)
	{
		bullet->updata();
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

	if (isInRange)
	{
		nextShotTime++;
	}

	if (nextShotTime >= 120)
	{
		isShot = true;
	}

	if (isShot)
	{
		std::unique_ptr<NormalBullet> newBullet = std::make_unique<NormalBullet>();
		newBullet->init({ 1,0,0,1 }, { 1,0,0,1 });

		XMFLOAT3 rampageTargetPos =
		{
			playerPosition.x - (float)(rand() % 8 - 4),
			playerPosition.y - (float)(rand() % 8 - 4),
			playerPosition.z - (float)(rand() % 8 - 4)
		};

		newBullet->bulletSpeed = 0.9f;
		newBullet->set(this->position, rampageTargetPos);
		normalBullets.push_back(std::move(newBullet));

		nextShotTime = 0;
		isShot = false;
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

	if (!Enemy::playerIsAlive)
	{
		return;
	}

	normalBullets.remove_if([](std::unique_ptr<NormalBullet>& bullet)
		{
			return bullet->isAlive == false;
		});

	for (std::unique_ptr<NormalBullet>& bullet : normalBullets)
	{
		bullet->updata();
	}

	if (isRampageWait)
	{
		rampageWaitCount++;

		if (rampageWaitCount >= 80)
		{
			isRampageWait = false;
		}
		return;
	}

	nextBulletTime++;

	if (nextBulletTime % 15 == 0)
	{
		std::unique_ptr<NormalBullet> newBullet = std::make_unique<NormalBullet>();
		newBullet->init({ 1,0,0,1 }, { 1,0,0,1 });

		XMFLOAT3 rampageTargetPos =
		{
			playerPosition.x - (float)(rand() % 8 - 4),
			playerPosition.y - (float)(rand() % 8 - 4),
			playerPosition.z - (float)(rand() % 8 - 4)
		};

		newBullet->bulletSpeed = 0.9f;
		newBullet->set(this->position, rampageTargetPos);
		normalBullets.push_back(std::move(newBullet));

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

void Boss::bossDraw3D(directX* directx)
{
	draw3D(directx);
}

void Boss::bossDraw2D(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	if (!playerIsAlive)
	{
		return;
	}

	if (isOutScreen)
	{
		outScreenIcon[0]->drawSprite(directx->cmdList.Get());
		outScreenIcon[1]->drawSprite(directx->cmdList.Get());
	}

	if (isTargetSet)
	{
		rockTarget->drawSprite(directx->cmdList.Get());
	}

	bossHitPointGauge.drawSprite(directx->cmdList.Get());
}