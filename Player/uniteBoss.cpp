#include "uniteBoss.h"
#include <random>

//static変数(パーツ)
std::unique_ptr<Model> uniteParts::partsModel = std::make_unique<Model>();
XMFLOAT3* uniteParts::motherPosition = nullptr;
const float uniteParts::partsRadius = 15.0f;

//static変数(本体)
std::unique_ptr<Model> uniteBoss::uniteBossModel = std::make_unique<Model>();
Player* uniteBoss::playerPointer = nullptr;

#pragma region パーツ
void uniteParts::partsInit(int ID)
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

	miniMapEnemy.anchorpoint = { 0.5f,0.5f };
	miniMapEnemy.size = { 15,15 };
	miniMapEnemy.generateSprite("enemyIcon.png");

	enemyHeight.anchorpoint = { 0.5f,0.5f };
	enemyHeight.size = { 32,3 };
	enemyHeight.generateSprite("bossHPGauge.png");

	enemyObject = new object3dFBX;
	enemyObject->initialize();
	enemyObject->SetModel(partsModel.get());
	enemyObject->SetScale({ 0.7f,0.7f,0.7f });
	enemyObject->setColor({ 0.3f,0.9f,0.3f,1 });

	enemyCollision.radius = 2.0f;

	enemyMovePattern = enemyPattern::shot;

	enemyCollision.radius = 9.0f;
	deathRotSpeed = 0.1f;
	partsID = ID;
}

void uniteParts::setStaticData(XMFLOAT3* motherposition)
{
	//本体の座標をセット
	motherPosition = motherposition;

	//モデル読み込み
	partsModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_02"));
}

void uniteParts::partsUpdata()
{
	//登場演出
	partsArrival();

	//弾の更新
	normalBullets.remove_if([](std::unique_ptr<NormalBullet>& bullet)
		{
			return bullet->isAlive == false;
		});

	for (std::unique_ptr<NormalBullet>& bullet : normalBullets)
	{
		bullet->updata();
	}

	position = enemyObject->getPosition();
	enemyObject->SetPosition(position);
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

	if (toPlayerAngle > 90)
	{
		//画面外(自分の位置がプレイヤーの向きと直角以上)にいるとき画面外フラグを立てる
		isOutScreen = true;
	}
	else
	{
		isOutScreen = false;
	}

	//生存時処理
	partsAriveMove();

	//撃墜演出
	partsDeathMove();
}

void uniteParts::partsSpriteUpdata()
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

	rockTarget->spriteUpdata();

	outScreenIcon[0]->spriteUpdata();
	outScreenIcon[1]->spriteUpdata();
}

void uniteParts::partsArrival()
{
	if (!isAppear)
	{
		return;
	}

	//本体の周りを回転する
	anglePhi += angleSpeed;
	angleTheta += angleSpeed;

	if (anglePhi > 360)
	{
		anglePhi -= 360;
	}

	if (angleTheta > 360)
	{
		angleTheta -= 360;
	}

	position = enemyObject->getPosition();

	angleThetaRad = angleTheta * (pi / radiannum);
	anglePhiRad = anglePhi * (pi / radiannum);

	position =
	{
		motherPosition->x + partsRadius * sinf(angleTheta) * cosf(anglePhi),
		motherPosition->y + partsRadius * sinf(angleTheta) * sinf(anglePhi),
		motherPosition->z + partsRadius * cosf(angleTheta)
	};

	enemyObject->SetPosition(position);
	enemyObject->updata();
}

void uniteParts::partsAriveMove()
{
	if (!isAlive || isAppear)
	{
		return;
	}

	position = enemyObject->getPosition();

	//本体の周りを回転する
	anglePhi += angleSpeed;
	angleTheta += angleSpeed;

	if (anglePhi > 360)
	{
		anglePhi -= 360;
	}

	if (angleTheta > 360)
	{
		angleTheta -= 360;
	}

	angleThetaRad = angleTheta * (pi / radiannum);
	anglePhiRad = anglePhi * (pi / radiannum);

	position =
	{
		motherPosition->x + partsRadius * sinf(angleTheta) * cosf(anglePhi),
		motherPosition->y + partsRadius * sinf(angleTheta) * sinf(anglePhi),
		motherPosition->z + partsRadius * cosf(angleTheta)
	};

	enemyObject->SetPosition(position);
	enemyObject->updata();

	//HPが0になったら消滅
	if (HP <= 0)
	{
		isAlive = false;
		fallDownCount = 0;
		angleTheta = 0;
		anglePhi = 0;

		enemyObject->setParent(nullptr);

#pragma region 爆発パーティクル生成
		SingleParticle newp;
		newp.generate();
		newp.set(maxFallCount - 20, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 0.2f, 10.0f);
		particleManagerOnTime::addParticle(newp, "bomb.png");
#pragma endregion 爆発パーティクル生成

		//弾も消す
		normalBullets.clear();
	}

	//当たり判定更新
	enemyObject->SetPosition(position);
	enemyCollision.center =
	{
		enemyObject->getPosition().x,
		enemyObject->getPosition().y,
		enemyObject->getPosition().z,1.0f
	};

	//スプライト更新
	updataSprite();
}

void uniteParts::partsDeathMove()
{
	if (isAlive || isAppear)
	{
		return;
	}

	position = enemyObject->getPosition();

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

void uniteParts::partsShotBullet(XMFLOAT3 targetposition)
{
	//弾の発射
	std::unique_ptr<NormalBullet> newBullet = std::make_unique<NormalBullet>();

	std::random_device seed;
	std::mt19937 rnd(seed());

	std::uint32_t redResult = rnd();
	std::uint32_t greenResult = rnd();
	std::uint32_t blueResult = rnd();

	XMFLOAT4 setColor =
	{
		(float)(redResult % 10) / 10,
		(float)(greenResult % 10) / 10,
		(float)(blueResult % 10) / 10,
		1
	};

	newBullet->init(setColor, setColor);
	newBullet->bulletSpeed = 0.9f;
	newBullet->set(this->position, targetposition);

	normalBullets.push_back(std::move(newBullet));
}

void uniteParts::partsSet(XMFLOAT3 position, float theta, float phi)
{
	this->position = position;
	anglePhi = phi;
	angleTheta = theta;
	HP = 6;

	isDraw = true;
	isAppear = true;
	isTargetSet = false;
	isStop = true;
	isRampageWait = true;
	chaseCount = 0;
	waitCount = 0;
	rampageWaitCount = 0;
	nextShotTime = 0;
	normalBullets.clear();

	this->position =
	{
		motherPosition->x + partsRadius * sinf(angleTheta) * cosf(anglePhi),
		motherPosition->y + partsRadius * sinf(angleTheta) * sinf(anglePhi),
		motherPosition->z + partsRadius * cosf(angleTheta)
	};

	enemyObject->SetPosition(this->position);
	enemyObject->updata();

	changePattern(enemyPattern::rampage);
}

void uniteParts::partsDraw3D(directX* directx)
{
	draw3D(directx);
}

void uniteParts::partsDraw2D(directX* directx)
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
#pragma endregion パーツ

#pragma region 本体
uniteBoss::uniteBoss()
{
}

uniteBoss::~uniteBoss()
{
}

//静的初期化
void uniteBoss::uniteBossStaticInit(Player* player)
{
	uniteBossModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("boss"));

	playerPointer = player;
}

//初期化
void uniteBoss::uniteBossInit()
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

	motherHitPointGauge.anchorpoint = { 0.5f,0.5f };
	motherHitPointGauge.size = { 50,20 };
	motherHitPointGauge.position = { 640,40,0 };
	motherHitPointGauge.generateSprite("bossHPGauge.png");

	allPartsHitPointGauge.anchorpoint = { 0.5f,0.5f };
	allPartsHitPointGauge.size = { 50,20 };
	allPartsHitPointGauge.position = { 640,40,0 };
	allPartsHitPointGauge.generateSprite("partsHPGauge.png");

	miniMapEnemy.anchorpoint = { 0.5f,0.5f };
	miniMapEnemy.size = { 15,15 };
	miniMapEnemy.generateSprite("enemyIcon.png");

	enemyHeight.anchorpoint = { 0.5f,0.5f };
	enemyHeight.size = { 32,3 };
	enemyHeight.generateSprite("bossHPGauge.png");

	enemyObject = new object3dFBX;
	enemyObject->initialize();
	enemyObject->SetModel(uniteBossModel.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });
	enemyObject->setColor({ 0.8f,0.8f,0.8f,1 });

	enemyCollision.radius = 2.0f;

	HP = 5;
	uniteBossScale = { 4,4,4 };
	enemyCollision.radius = 9.0f;
	deathRotSpeed = 0.1f;

	uniteParts::setStaticData(&position);
}

//更新
void uniteBoss::uniteBossUpdata()
{
	enemyObject->updata();

	uniteBossArrival();

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
	XMFLOAT3 uniteBossPosition = position;

	//上から見た位置なので X・Z座標
	XMFLOAT3 minimapPosition =
	{
		(uniteBossPosition.x * 0.21f) + Enemy::miniMapPosition.x,
		(uniteBossPosition.z * 0.21f) + Enemy::miniMapPosition.x,
		0
	};

	miniMapEnemy.position = minimapPosition;
	miniMapEnemy.spriteUpdata();

	uniteBossAriveMove();

	uniteBossDeathMove();
}

//スプライト更新
void uniteBoss::uniteBossSpriteUpdata()
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

	rockTarget->spriteUpdata();

	outScreenIcon[0]->spriteUpdata();
	outScreenIcon[1]->spriteUpdata();

	motherHitPointGauge.size.x = (float)HP * 70;
	motherHitPointGauge.spriteUpdata();

	int allPartsHP = 0;

	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		allPartsHP += parts->HP;
	}

	allPartsHitPointGauge.size.x = (float)allPartsHP * 14;
	allPartsHitPointGauge.spriteUpdata();

	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsSpriteUpdata();
	}

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

//登場演出
void uniteBoss::uniteBossArrival()
{
	if (!isAppear)
	{
		return;
	}

	playerPointer->isStop = true;
	playerPointer->isInvisible = 1;

	isStop = true;

	position.y -= 0.2f;
	enemyObject->SetPosition(position);
	enemyObject->updata();

	uniteBossCamera->SetTarget(position);
	uniteBossCamera->Update();

	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsUpdata();
	}

	if (position.y <= arrivalEndPos.y)
	{
		enemyObject->SetRotation({ 0,0,0 });
		enemyObject->SetPosition(arrivalEndPos);
		enemyObject->updata();
		isStop = false;
		isAlive = true;
		isAppear = false;
		isArmor = true;
		playerPointer->isStop = false;
		playerPointer->isInvisible = -1;
		isSelectPattern = true;

		for (std::unique_ptr<uniteParts>& parts : partsList)
		{
			parts->partsUpdata();
			parts->isStop = false;
			parts->isAlive = true;
			parts->isAppear = false;
			parts->angleSpeed = 0.01f;
		}

		object3dFBX::SetCamera(playerPointer->followCamera);
	}
}

//生存時処理
void uniteBoss::uniteBossAriveMove()
{
	if (!isAlive || isAppear)
	{
		return;
	}

	//行動パターンの抽選
	selectMovePattern();

	//突進
	uniteBossChargeAttack();

	//自機狙い弾
	uniteBossShotPlayerTarget();

	//弾乱射
	uniteBossRampage();

	//パーツの更新
	partsList.remove_if([](std::unique_ptr<uniteParts>& parts)
		{
			return parts->isDraw == false;
		});

	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsUpdata();
		if (!parts->isAlive)
		{
			parts->normalBullets.clear();
		}
	}

	//全てのパーツが破壊されたかどうか
	if (partsList.size() <= 0)
	{
		isAllPartsBreak = true;
		isArmor = false;
	}

	//HPが0になったら消滅
	if (HP <= 0)
	{
		isAlive = false;
		fallDownCount = 0;
		deathRotSpeed = 0.5f;
		CameraAngleSpeed = 0.0f;

		//弾も消す
		normalBullets.clear();
	}

	uniteBossSpriteUpdata();

	enemyObject->SetPosition(position);
	enemyCollision.center =
	{
		enemyObject->getPosition().x,
		enemyObject->getPosition().y,
		enemyObject->getPosition().z,1.0f
	};
}

//撃墜処理
void uniteBoss::uniteBossDeathMove()
{
	if (isAlive || isAppear)
	{
		return;
	}

	playerPointer->isStop = true;
	playerPointer->isInvisible = 1;

	position = enemyObject->getPosition();

	//ボスの周囲をカメラが回転する
	XMFLOAT3 setvec =
	{
		position.x + (cosf(CameraAngleSpeed * (M_PI / 180.0f)) * 30),
		position.y - 5,
		position.z + (sinf(CameraAngleSpeed * (M_PI / 180.0f)) * 30),
	};

	CameraAngleSpeed += 0.8f;

	uniteBossCamera->SetEye(setvec);
	uniteBossCamera->SetTarget(position);
	uniteBossCamera->Update();

	object3dFBX::SetCamera(uniteBossCamera);
	SingleParticle::setCamera(uniteBossCamera);

	//撃墜演出のカウント
	fallDownCount++;

	float scale = 1.0f - ((float)fallDownCount / ((float)maxFallCount * 7));

	rot.x += deathRotSpeed;
	rot.y += deathRotSpeed;
	rot.z += deathRotSpeed;
	enemyObject->setRotMatrix(rot.x, rot.y, rot.z);
	enemyObject->SetScale({ uniteBossScale.x * scale,uniteBossScale.y * scale,uniteBossScale.z * scale });

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

	//演出が終わったら
	if (fallDownCount >= maxFallCount * 5)
	{
		isDraw = false;
		fallDownCount = 0;
	}
}

//セット
void uniteBoss::uniteBossSet()
{
	arrivalStartPos = { 0,80,0 };
	arrivalEndPos = { 0,5,0 };
	position = arrivalStartPos;
	isTargetSet = false;
	isDraw = true;

	isChase = false;
	isWait = true;
	chaseCount = 0;
	waitCount = 0;
	chargeAttackCount = 0;
	nextBulletTime = 0;
	bulletCount = 0;
	maxBulletCount = 10;
	isRampageWait = true;
	rampageWaitCount = 0;
	normalBullets.clear();

	HP = resetHitPoint;
	arrivalTime = 300;
	enemyObject->SetScale(uniteBossScale);
	enemyObject->SetPosition({ 0,0,0 });
	enemyObject->updata();

	//演出用カメラをセット
	uniteBossCamera = new Camera;
	uniteBossCamera->SetEye({ 0,0,position.z - 35 });
	uniteBossCamera->SetTarget(position);
	object3dFBX::SetCamera(uniteBossCamera);

	partsList.clear();

	//パーツ初期化＆親オブジェクト設定
	for (int i = 0; i < unitNum; i++)
	{
		std::unique_ptr<uniteParts> newParts = std::make_unique<uniteParts>();
		newParts->partsInit(i);
		newParts->partsSet({ 0,0,0 }, (float)(rand() % 360), (float)(rand() % 360));
		newParts->angleSpeed = 0.05f;
		partsList.push_back(std::move(newParts));
	}

	isSelectPattern = true;
	isAppear = true;
}

//行動パターン抽選
void uniteBoss::selectMovePattern()
{
	if (!isSelectPattern)
	{
		return;
	}

	uniteBossNowPattern = (uniteBossPattern)(rand() % 3);

	isSelectPattern = false;
}

//突進
void uniteBoss::uniteBossChargeAttack()
{
	if (uniteBossNowPattern != uniteBossPattern::chargeAttack)
	{
		return;
	}

	if (isAppear || isSelectPattern)
	{
		return;
	}

	if (!Enemy::playerIsAlive)
	{
		return;
	}

	if (chargeAttackCount >= maxChargeAttackCount)
	{
		isChase = false;
		chaseCount = 0;
		waitCount = 0;
		isWait = true;
		isSelectPattern = true;
		chargeAttackCount = 0;
	}

	//追跡
	if (isChase)
	{
		//追尾カウント加算
		chaseCount++;
		enemySpeed = 1.5f;
		if (chaseCount >= 1)
		{
			isChase = false;
			chaseCount = 0;
			isWait = true;
		}
	}

	//減速
	if (isWait)
	{
		//待機カウント加算
		waitCount++;
		if (enemySpeed > 0.0f)
		{
			enemySpeed -= 0.03f;
		}
		else
		{
			chargeAttackCount++;
			isWait = false;
			waitCount = 0;
			enemySpeed = 0.0f;
			isChase = true;
		}
	}

	if (isChase)
	{
		XMFLOAT3 pPos = playerPointer->playerObject->getPosition();

		position = enemyObject->getPosition();
		chargeAttackVec = { pPos.x - position.x,pPos.y - position.y,pPos.z - position.z };

		float lengthDis = sqrtf(powf(chargeAttackVec.x, 2) + powf(chargeAttackVec.y, 2) + powf(chargeAttackVec.z, 2));

		chargeAttackVec.x /= lengthDis;
		chargeAttackVec.y /= lengthDis;
		chargeAttackVec.z /= lengthDis;
	}

	position.x += chargeAttackVec.x * enemySpeed;
	position.y += chargeAttackVec.y * enemySpeed;
	position.z += chargeAttackVec.z * enemySpeed;

	enemyObject->SetPosition(position);
}

//乱射
void uniteBoss::uniteBossRampage()
{
	if (uniteBossNowPattern != uniteBossPattern::rampageBullet)
	{
		return;
	}

	if (isAppear || isSelectPattern)
	{
		return;
	}

	if (!Enemy::playerIsAlive)
	{
		return;
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

	if (nextBulletTime % 3 == 0)
	{
		//パーツから弾を発射
		for (std::unique_ptr<uniteParts>& parts : partsList)
		{
			XMFLOAT3 target =
			{
				parts->position.x + (parts->position.x - position.x),
				parts->position.y + (parts->position.y - position.y),
				parts->position.z + (parts->position.z - position.z),
			};

			parts->partsShotBullet(target);

			parts->angleSpeed = 0.08f;
		}

		bulletCount++;
	}

	if (bulletCount == maxBulletCount)
	{
		isRampageWait = true;
		rampageWaitCount = 0;
		nextBulletTime = 0;
		bulletCount = 0;
		shotCount++;
		if (shotCount >= maxRanpageCount)
		{
			shotCount = 0;
			isSelectPattern = true;

			for (std::unique_ptr<uniteParts>& parts : partsList)
			{
				parts->angleSpeed = 0.01f;
			}
		}
	}
}

//自機狙い弾
void uniteBoss::uniteBossShotPlayerTarget()
{
	if (uniteBossNowPattern != uniteBossPattern::shotPlayerTarget)
	{
		return;
	}

	if (isAppear || isSelectPattern)
	{
		return;
	}

	if (!Enemy::playerIsAlive)
	{
		return;
	}

	if (shotCount >= maxShotCount)
	{
		shotCount = 0;
		nextShotTime = 0;
		isSelectPattern = true;
	}

	XMFLOAT3 pPos = playerPointer->playerObject->getPosition();

	XMFLOAT3 startToTarget =
	{
		pPos.x - this->position.x,
		pPos.y - this->position.y,
		pPos.z - this->position.z
	};

	//本体からプレイヤーへの距離を計算
	float length = sqrtf(powf(startToTarget.x, 2) + powf(startToTarget.y, 2) + powf(startToTarget.z, 2));

	//射程範囲内か判断
	if (length <= forPlayer)
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

	if (nextShotTime >= 60)
	{
		isShot = true;
		nextShotTime = 0;
	}
	else
	{
		isShot = false;
	}

	//射撃
	if (isShot)
	{
		//パーツから弾を発射
		for (std::unique_ptr<uniteParts>& parts : partsList)
		{
			parts->partsShotBullet(pPos);
		}
		shotCount++;
		isShot = false;
	}
}

//描画_3D
void uniteBoss::uniteBossDraw3d(directX* directx)
{
	draw3D(directx);

	//パーツの描画
	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsDraw3D(directx);
	}
}

//描画_2D
void uniteBoss::uniteBossDraw2d(directX* directx)
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

	motherHitPointGauge.drawSprite(directx->cmdList.Get());
	allPartsHitPointGauge.drawSprite(directx->cmdList.Get());

	//パーツの描画
	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsDraw2D(directx);
	}
}
#pragma endregion 本体
