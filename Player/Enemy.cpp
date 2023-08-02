#include"Enemy.h"
#include<random>

std::unique_ptr<Model> Enemy::enemyModelS = std::make_unique<Model>();
XMFLOAT3 Enemy::playerPosition = { 0,0,0 };
XMFLOAT3 Enemy::playerFront = { 0,0,0 };
bool Enemy::playerIsAlive = false;
const float Enemy::forPlayer = 400.0f;
const XMFLOAT2 Enemy::defaultRockIconSize = { 70.0f,70.0f };
const XMFLOAT2 Enemy::setRockIconSize = { 180.0f,180.0f };
const float Enemy::decreaseSize = 9.0f;
const XMFLOAT3 Enemy::miniMapPosition = { -90,120,0 };

#pragma region 敵本体
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(enemyObject);
}

void Enemy::staticInit()
{
	enemyModelS.reset(FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_02"));

	SingleParticle::loadTexInMap("bomb.png");
	SingleParticle::loadTexInMap("smoke.png");
	SingleParticle::loadTexInMap("enemy_chip.png");
}

void Enemy::staticUpdata(XMFLOAT3 playerpos, XMFLOAT3 playerfront, bool playerisarive)
{
	playerPosition = playerpos;
	playerFront = playerfront;
	playerIsAlive = playerisarive;
}

void Enemy::staticDestroy()
{
}

void Enemy::init(enemyPattern pattern)
{
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

	miniMapEnemy.anchorpoint = { 0.5f,0.5f };
	miniMapEnemy.size = { 15,15 };
	miniMapEnemy.generateSprite("enemyIcon.png");

	enemyHeight.anchorpoint = { 0.5f,0.5f };
	enemyHeight.size = { 32,3 };
	enemyHeight.generateSprite("bossHPGauge.png");

	enemyObject = new object3dFBX;
	enemyObject->initialize();
	enemyObject->SetModel(enemyModelS.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });

	enemyCollision.radius = 2.0f;

	enemyMovePattern = pattern;
}

void Enemy::set(XMFLOAT3 pos)
{
	position = pos;
	startPosition = pos;
	enemyObject->SetPosition(pos);
	isAlive = false;
	isTargetSet = false;
	chaseCount = 0;
	waitCount = 0;
	isChase = false;
	isWait = true;
	isDraw = true;
	enemyArrivalTime = 100;
	enemyArrivaCount = 0;
	arrivalEase.set(easingType::easeOut, easingPattern::Quadratic, enemyArrivalTime, 500, 0);
	nextBulletTime = 0;
	bulletCount = 0;
	isRampageWait = true;
	rampageWaitCount = 0;
	normalBullets.clear();

	isAppear = true;
}

void Enemy::changePattern(enemyPattern pattern)
{
	enemyMovePattern = pattern;

	switch (enemyMovePattern)
	{
	case enemyPattern::tutorial:
		bodyColor = { 1,1,1,1 };
		break;

	case enemyPattern::chase:
		bodyColor = { 0.3f,1,0.3f,1 };
		break;

	case enemyPattern::shot:
		bodyColor = { 0.2f,0.2f,1,1 };
		break;

	case enemyPattern::homing:
		bodyColor = { 0,1,1,1 };
		break;

	case enemyPattern::rampage:
		bodyColor = { 1,0.2f,0.2f,1 };
		break;

	default:
		break;
	}

	enemyObject->setColor(bodyColor);
}

void Enemy::reSet()
{
	enemyObject->SetPosition(startPosition);
	position = startPosition;
	chaseCount = 0;
	waitCount = 0;
	isChase = false;
	isWait = true;
	isAlive = true;
	HP = 1;
	isTargetSet = false;
	isSetMissile = false;
	isDraw = true;
}

void Enemy::tutorial()
{
	if (enemyMovePattern != enemyPattern::tutorial)
	{
		return;
	}

	enemyObject->updata();
}

void Enemy::chase()
{
	if (enemyMovePattern != enemyPattern::chase)
	{
		return;
	}

	if (!playerIsAlive)
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

	position = enemyObject->getPosition();
	XMFLOAT3 dis =
	{
		playerPosition.x - position.x,
		playerPosition.y - position.y,
		playerPosition.z - position.z
	};

	float lengthDis = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	dis.x /= lengthDis;
	dis.y /= lengthDis;
	dis.z /= lengthDis;

	position.x += dis.x * enemySpeed;
	position.y += dis.y * enemySpeed;
	position.z += dis.z * enemySpeed;
}

void Enemy::shot()
{
	if (enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	if (!playerIsAlive)
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

	//射程範囲かどうかを計算
	XMFLOAT3 startToTarget =
	{
		playerPosition.x - this->position.x,
		playerPosition.y - this->position.y,
		playerPosition.z - this->position.z
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

	//弾を撃っていないかつ射程範囲内なら射撃までのカウントダウンを進める
	if (isInRange)
	{
		nextShotTime++;
		scale += 0.005f;
	}

	//一定カウントごとに射撃フラグを立てる
	if (nextShotTime >= 120)
	{
		scale = 1.0f;
		isShot = true;
	}

	//弾を撃つ
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

void Enemy::homing()
{
	if (enemyMovePattern != enemyPattern::homing)
	{
		return;
	}

	if (!playerIsAlive)
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

	XMFLOAT3 startToTarget =
	{
		playerPosition.x - this->position.x,
		playerPosition.y - this->position.y,
		playerPosition.z - this->position.z
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
		enemySpeed = 0.25f;
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
	XMFLOAT3 dis =
	{
		playerPosition.x - position.x,
		playerPosition.y - position.y,
		playerPosition.z - position.z
	};

	float lengthDis = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	dis.x /= lengthDis;
	dis.y /= lengthDis;
	dis.z /= lengthDis;

	position.x += dis.x * enemySpeed;
	position.y += dis.y * enemySpeed;
	position.z += dis.z * enemySpeed;
}

void Enemy::rampage()
{
	if (enemyMovePattern != enemyPattern::rampage)
	{
		return;
	}

	if (!playerIsAlive)
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

		if (rampageWaitCount >= 100)
		{
			isRampageWait = false;
		}
		return;
	}

	nextBulletTime++;

	if (nextBulletTime % 10 == 0)
	{
		std::unique_ptr<NormalBullet> newBullet = std::make_unique<NormalBullet>();
		newBullet->init({ 1,0,0,1 }, { 1,0,0,1 });

		XMFLOAT3 rampageTargetPos =
		{
			playerPosition.x - (float)(rand() % 8 - 4),
			playerPosition.y - (float)(rand() % 8 - 4),
			playerPosition.z - (float)(rand() % 8 - 4)
		};

		newBullet->bulletSpeed = 0.5f;
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

void Enemy::updata()
{
	if (!isDraw)
	{
		return;
	}

	arrival();

	enemyObject->updata();

	if (isStop)
	{
		return;
	}

	//プレイヤーとの位置関係を計算
	XMFLOAT3 playerToEnemy =
	{
		position.x - playerPosition.x,
		position.y - playerPosition.y,
		position.z - playerPosition.z,
	};

	float length = sqrtf(powf(playerToEnemy.x, 2) + powf(playerToEnemy.y, 2) + powf(playerToEnemy.z, 2));

	//プレイヤーと距離が離れすぎているか
	if (length >= forPlayer)
	{
		isFar = true;
	}
	else
	{
		isFar = false;
	}

	//ベクトルの内積から角度を求めて、ロックオンの範囲を絞る
	float dot = playerToEnemy.x * playerFront.x + playerToEnemy.y * playerFront.y + playerToEnemy.z * playerFront.z;

	float vecLen = sqrtf((powf(playerToEnemy.x, 2) + powf(playerToEnemy.y, 2) + powf(playerToEnemy.z, 2)) * (powf(playerFront.x, 2) + powf(playerFront.y, 2) + powf(playerFront.z, 2)));

	toPlayerAngle = acosf(dot / vecLen) * (180.0f / (float)M_PI);

	if (toPlayerAngle > 90 && !isAppear)
	{
		//画面外(自分の位置がプレイヤーの向きと直角以上)にいるとき画面外フラグを立てる
		isOutScreen = true;
	}
	else
	{
		isOutScreen = false;
	}

	//敵が生存
	ariveMove();

	//敵が撃墜
	deathMove();

	enemyObject->SetScale({ scale ,scale ,scale });
	enemyObject->SetPosition(position);

	return;
}

void Enemy::updataSprite()
{
	XMFLOAT2 targetPos = enemyObject->worldToScleen();

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
	if (targetPos.x < outScreenIcon[0]->size.x / 2)
	{
		targetPos.x = outScreenIcon[0]->size.x / 2;
	}
	else if (targetPos.x > 1280 - outScreenIcon[0]->size.x / 2)
	{
		targetPos.x = 1280 - outScreenIcon[0]->size.x / 2;
	}
	if (targetPos.y < outScreenIcon[0]->size.y / 2)
	{
		targetPos.y = outScreenIcon[0]->size.y / 2;
	}
	else if (targetPos.y > 720 - outScreenIcon[0]->size.y / 2)
	{
		targetPos.y = 720 - outScreenIcon[0]->size.y / 2;
	}

	//プレイヤーの視界外に行ったら、画面の下にアイコンを移動させる
	if (isOutScreen)
	{
		targetPos.y = 720 - outScreenIcon[0]->size.y / 2;
	}

	rockTarget->position = { targetPos.x,targetPos.y,0 };
	outScreenIcon[0]->position = { targetPos.x,targetPos.y,0 };
	outScreenIcon[1]->position = { targetPos.x,targetPos.y,0 };

	rockTarget->spriteUpdata();

	outScreenIcon[0]->spriteUpdata();
	outScreenIcon[1]->spriteUpdata();

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

void Enemy::arrival()
{
	if (!isAppear)
	{
		return;
	}

	isDraw = true;

	enemyArrivaCount++;

	arrivalScale.x = (float)enemyArrivaCount / (float)enemyArrivalTime;
	arrivalScale.y = (float)enemyArrivaCount / (float)enemyArrivalTime;
	arrivalScale.z = (float)enemyArrivaCount / (float)enemyArrivalTime;

	float rot = arrivalEase.easing();

	XMMATRIX matrot = XMMatrixIdentity();
	matrot *= XMMatrixRotationZ(XMConvertToRadians(rot));
	matrot *= XMMatrixRotationX(XMConvertToRadians(rot));
	matrot *= XMMatrixRotationY(XMConvertToRadians(rot));

	enemyObject->setRotMatrix(matrot);
	enemyObject->SetPosition(position);
	enemyObject->SetScale(arrivalScale);
	enemyObject->updata();

	if (!arrivalEase.getIsActive())
	{
		enemyObject->SetRotation({ 0,0,0 });
		enemyObject->SetPosition(position);
		enemyObject->SetScale({ 1,1,1 });
		enemyObject->updata();
		enemyArrivaCount = 0;
		isStop = false;
		isAlive = true;
		isAppear = false;
	}
}

void Enemy::ariveMove()
{
	if (!isAlive || isAppear)
	{
		return;
	}

	tutorial();

	chase();

	shot();

	homing();

	rampage();

	//HPが0になったら消滅
	if (HP <= 0)
	{
		isAlive = false;
		fallDownCount = 0;

#pragma region 爆発パーティクル生成
		SingleParticle newp;
		newp.generate();
		newp.set(maxFallCount - 20, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 0.2f, 10.0f);
		particleManagerOnTime::addParticle(newp, "bomb.png");
#pragma endregion 爆発パーティクル生成

#pragma region 破片パーティクル生成
		for (int i = 0; i < 7; i++)
		{
			std::random_device seed;
			std::mt19937 rnd(seed());

			std::uint32_t xVecResult = (rnd() % 10) - 5;
			std::uint32_t zVecResult = (rnd() % 10) - 5;

			SingleParticle newp;
			newp.generate();
			newp.color = bodyColor;
			newp.set(maxFallCount - 30, enemyObject->getPosition(),
				{ (float)xVecResult / 5,0.5f,(float)zVecResult / 5 },
				{ 0,-0.01,0 }, 1.5f, 1.5f);
			particleManagerOnTime::addParticle(newp, "enemy_chip.png");
		}
#pragma endregion 破片パーティクル生成

		//弾も消す
		normalBullets.clear();
	}

	enemyObject->SetPosition(position);
	enemyCollision.center =
	{
		enemyObject->getPosition().x,
		enemyObject->getPosition().y,
		enemyObject->getPosition().z,1.0f
	};

	updataSprite();
}

void Enemy::deathMove()
{
	if (isAlive || isAppear)
	{
		return;
	}

	fallDownCount++;

	position.y -= 0.2f;
	rot.x += deathRotSpeed;
	rot.y += deathRotSpeed;
	rot.z += deathRotSpeed;
	enemyObject->setRotMatrix(rot.x, rot.y, rot.z);

#pragma region 黒煙パーティクル
	if (fallDownCount % 15 == 0)
	{
		SingleParticle newp;
		newp.generate();
		newp.set((maxFallCount - fallDownCount) + 10, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 3.5, 0.5);
		particleManagerOnTime::addParticle(newp, "smoke.png");
	}
#pragma endregion 黒煙パーティクル

	//落ちきったら
	if (fallDownCount >= maxFallCount)
	{
		isDraw = false;
		fallDownCount = 0;
	}
}

void Enemy::draw3D(directX* directx)
{
	if (!isDraw)
	{
		return;
	}

	if (isFar)
	{
		return;
	}

	if (!playerIsAlive)
	{
		return;
	}

	enemyObject->Draw(directx->cmdList.Get());

	for (std::unique_ptr<NormalBullet>& bullet : normalBullets)
	{
		bullet->draw(directx);
	}
}

void Enemy::draw2D(directX* directx)
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
}

void Enemy::drawMiniMapIcon(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	if (!playerIsAlive)
	{
		return;
	}

	miniMapEnemy.drawSprite(directx->cmdList.Get());

	enemyHeight.drawSprite(directx->cmdList.Get());
}
#pragma endregion