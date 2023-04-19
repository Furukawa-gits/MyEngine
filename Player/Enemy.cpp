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
const XMFLOAT3 Enemy::miniMapPosition = { 120,120,0 };

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

	enemyBullet::staticInit();
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
	miniMapEnemy.size = { 4,4 };
	miniMapEnemy.generateSprite("bossHPGauge.png");

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
	bomParticles.clear();
	smokeParticles.clear();
	nextBulletTime = 0;
	bulletCount = 0;
	isRampageWait = true;
	rampageWaitCount = 0;
	Bullets.clear();

	isAppear = true;
}

void Enemy::changePattern(enemyPattern pattern)
{
	enemyMovePattern = pattern;

	switch (enemyMovePattern)
	{
	case enemyPattern::tutorial:
		enemyObject->setColor({ 1,1,1,1 });
		break;

	case enemyPattern::chase:
		enemyObject->setColor({ 0.3f,1,0.3f,1 });
		break;

	case enemyPattern::shot:
		enemyObject->setColor({ 0.2f,0.2f,1,1 });
		break;

	case enemyPattern::homing:
		enemyObject->setColor({ 0,1,1,1 });
		break;

	case enemyPattern::rampage:
		enemyObject->setColor({ 1,0.2f,0.2f,1 });
		break;

	default:
		break;
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

	Bullets.remove_if([](std::unique_ptr<enemyBullet>& bullet)
		{
			return bullet->isBulletArive() == false;
		});

	for (std::unique_ptr<enemyBullet>& bullet : Bullets)
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
		std::unique_ptr<enemyBullet> newBullet = std::make_unique<enemyBullet>();
		newBullet->init();

		XMFLOAT3 rampageTargetPos =
		{
			playerPosition.x - (float)(rand() % 8 - 4),
			playerPosition.y - (float)(rand() % 8 - 4),
			playerPosition.z - (float)(rand() % 8 - 4)
		};

		newBullet->set(rampageTargetPos, this->position);
		Bullets.push_back(std::move(newBullet));

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
	Bullets.remove_if([](std::unique_ptr<enemyBullet>& bullet)
		{
			return bullet->isBulletArive() == false;
		});

	for (std::unique_ptr<enemyBullet>& bullet : Bullets)
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
		std::unique_ptr<enemyBullet> newBullet = std::make_unique<enemyBullet>();
		newBullet->init();

		XMFLOAT3 rampageTargetPos =
		{
			playerPosition.x - (float)(rand() % 8 - 4),
			playerPosition.y - (float)(rand() % 8 - 4),
			playerPosition.z - (float)(rand() % 8 - 4)
		};

		newBullet->set(rampageTargetPos, this->position);
		Bullets.push_back(std::move(newBullet));

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

	Bullets.remove_if([](std::unique_ptr<enemyBullet>& bullet)
		{
			return bullet->isBulletArive() == false;
		});

	for (std::unique_ptr<enemyBullet>& bullet : Bullets)
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
		std::unique_ptr<enemyBullet> newBullet = std::make_unique<enemyBullet>();
		newBullet->init();

		XMFLOAT3 rampageTargetPos =
		{
			playerPosition.x - (float)(rand() % 8 - 4),
			playerPosition.y - (float)(rand() % 8 - 4),
			playerPosition.z - (float)(rand() % 8 - 4)
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

	//ミニマップアイコン更新
	XMFLOAT3 enemyPosition = position;

	//上から見た位置なので X・Z座標
	XMFLOAT3 minimapPosition =
	{
		(enemyPosition.x * 0.21f) + Enemy::miniMapPosition.x,
		(enemyPosition.z * 0.21f) + Enemy::miniMapPosition.x,
		0
	};

	miniMapEnemy.position = minimapPosition;
	miniMapEnemy.spriteTransferVertexBuffer();
	miniMapEnemy.spriteUpdata();

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

	rockTarget->spriteTransferVertexBuffer();
	rockTarget->spriteUpdata();

	outScreenIcon[0]->spriteTransferVertexBuffer();
	outScreenIcon[0]->spriteUpdata();
	outScreenIcon[1]->spriteTransferVertexBuffer();
	outScreenIcon[1]->spriteUpdata();
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
		std::unique_ptr<SingleParticle> newparticle = std::make_unique<SingleParticle>();
		newparticle->generate();
		newparticle->set(maxFallCount - 20, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 0.2f, 10.0f);
		bomParticles.push_back(std::move(newparticle));
#pragma endregion 爆発パーティクル生成

		//弾も消す
		Bullets.clear();
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

	//爆発パーティクル更新
	bomParticles.remove_if([](std::unique_ptr<SingleParticle>& newparticle)
		{
			return newparticle->getIsActive() == false;
		});
	for (std::unique_ptr<SingleParticle>& newparticle : bomParticles)
	{
		newparticle->updata();
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
		std::unique_ptr<SingleParticle> newparticle = std::make_unique<SingleParticle>();
		newparticle->generate();
		newparticle->set((maxFallCount - fallDownCount) + 10, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 3.5, 0.5);

		smokeParticles.push_back(std::move(newparticle));
	}
#pragma endregion 黒煙パーティクル

	//煙パーティクル更新
	smokeParticles.remove_if([](std::unique_ptr<SingleParticle>& newparticle)
		{
			return newparticle->getIsActive() == false;
		});
	for (std::unique_ptr<SingleParticle>& newparticle : smokeParticles)
	{
		newparticle->updata();
	}

	//落ちきったら
	if (fallDownCount >= maxFallCount)
	{
		bomParticles.clear();
		smokeParticles.clear();
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

	for (std::unique_ptr<enemyBullet>& bullet : Bullets)
	{
		bullet->draw(directx);
	}

	//爆発パーティクル描画
	for (std::unique_ptr<SingleParticle>& newparticle : bomParticles)
	{
		newparticle->drawSpecifyTex("bomb.png");
	}

	for (std::unique_ptr<SingleParticle>& newparticle : smokeParticles)
	{
		newparticle->drawSpecifyTex("smoke.png");
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
}
#pragma endregion

#pragma region 敵の弾
enemyBullet::enemyBullet()
{
}

enemyBullet::~enemyBullet()
{
}

void enemyBullet::staticInit()
{
}

void enemyBullet::staticDestroy()
{
}

void enemyBullet::init()
{
	//親パーティクル生成
	motherParticle = std::make_unique<SingleParticle>();
	motherParticle->generate();
	motherParticle->set(0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f, false, true);
	motherParticle->color = { 1,0,0,1 };

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
	isAlive = true;
}

void enemyBullet::updata()
{
	if (!isAlive)
	{
		for (std::unique_ptr<SingleParticle>& newparticle : childParticles)
		{
			newparticle->setIsActive(false);
		}
		return;
	}

	ariveTime++;

	position.x += bulletVec.x * bulletSpeed;
	position.y += bulletVec.y * bulletSpeed;
	position.z += bulletVec.z * bulletSpeed;

	//一定フレームごとにパーティクルを生成
	if (ariveTime % 10 == 0)
	{
		std::unique_ptr<SingleParticle> newParticle = std::make_unique<SingleParticle>();
		newParticle->generate();
		newParticle->set(20, position, { 0,0,0 }, { 0,0,0 }, 2.0f, 0.0f);
		newParticle->color = motherParticle->color;
		childParticles.push_back(std::move(newParticle));
	}

	//サブパーティクル更新
	childParticles.remove_if([](std::unique_ptr<SingleParticle>& newparticle)
		{
			return newparticle->getIsActive() == false;
		});
	for (std::unique_ptr<SingleParticle>& newparticle : childParticles)
	{
		newparticle->updata();
	}

	//本体パーティクル更新
	motherParticle->setPosition(position);
	motherParticle->updata();

	bulletCollision.center = XMLoadFloat3(&position);

	if (ariveTime >= maxAriveTime)
	{
		motherParticle->setIsActive(false);
		isAlive = false;
		ariveTime = 0;
	}
}

void enemyBullet::draw(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	for (std::unique_ptr<SingleParticle>& newparticle : childParticles)
	{
		newparticle->setPiplineAddBlend();
		newparticle->drawSpecifyTex("effect1.png");
	}

	motherParticle->setPiplineAddBlend();
	motherParticle->drawSpecifyTex("effect1.png");
}
#pragma endregion