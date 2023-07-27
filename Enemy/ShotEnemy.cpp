#include "ShotEnemy.h"

ShotEnemy::ShotEnemy()
{
}

ShotEnemy::~ShotEnemy()
{
}

void ShotEnemy::init(bool isboss)
{
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

	miniMapEnemy = std::make_unique<SingleSprite>();
	miniMapEnemy->anchorpoint = { 0.5f,0.5f };
	miniMapEnemy->size = { 15,15 };
	miniMapEnemy->generateSprite("enemyIcon.png");

	enemyHeight = std::make_unique<SingleSprite>();
	enemyHeight->anchorpoint = { 0.5f,0.5f };
	enemyHeight->size = { 32,3 };
	enemyHeight->generateSprite("bossHPGauge.png");

	enemyObject = std::make_unique<object3dFBX>();
	enemyObject->initialize();
	enemyObject->SetModel(staticEnemyModel.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });

	myEnemyType = enemyType::shot;

	bodyColor = { 0.2f,0.2f,1,1 };
}

void ShotEnemy::set(XMFLOAT3 pos)
{
	position = pos;
	startPosition = pos;
	enemyObject->SetPosition(pos);
	isAlive = false;
	isTargetSet = false;
	isDraw = true;
	enemyArrivalTime = 100;
	enemyArrivaCount = 0;
	arrivalEase.set(easingType::easeOut, easingPattern::Quadratic, enemyArrivalTime, 500, 0);
	normalBullets.clear();

	isAppear = true;
}

void ShotEnemy::updata()
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

void ShotEnemy::arrival()
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

void ShotEnemy::ariveMove()
{
	if (!isAlive || isAppear)
	{
		return;
	}

	shot();

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

void ShotEnemy::deathMove()
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

void ShotEnemy::updataSprite()
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

	enemyHeight->position = { miniMapPosition.x + 120,enemyH,0 };
	miniMapEnemy->position = { miniMapPosition.x + 140,enemyH - 2,0 };

	miniMapEnemy->spriteUpdata();
	enemyHeight->spriteUpdata();
}

void ShotEnemy::draw3D()
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

void ShotEnemy::draw2D()
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

void ShotEnemy::drawMiniMapIcon()
{
	if (!isAlive)
	{
		return;
	}

	if (!playerIsAlive)
	{
		return;
	}

	miniMapEnemy->drawSprite(directx->cmdList.Get());

	enemyHeight->drawSprite(directx->cmdList.Get());
}

void ShotEnemy::shot()
{
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
