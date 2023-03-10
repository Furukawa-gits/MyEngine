#include "uniteBoss.h"

//static変数(パーツ)
XMFLOAT3* uniteParts::motherPosition = nullptr;
const float uniteParts::partsRadius = 15.0f;

//static変数(本体)
std::unique_ptr<Model> uniteBoss::uniteBossModel = std::make_unique<Model>();
Player* uniteBoss::playerPointer = nullptr;

#pragma region パーツ
void uniteParts::partsInit(int ID)
{
	//基底クラスの初期化
	init(enemyPattern::shot);
	enemyObject->setColor({ 1.0f,1.0f,1.0f,1 });
	isThisBoss = true;
	partsID = ID;
}

void uniteParts::setStaticData(XMFLOAT3* motherposition)
{
	//本体の座標をセット
	motherPosition = motherposition;
}

void uniteParts::partsUpdata()
{
	//登場演出
	partsArrival();

	//弾の更新
	Bullets.remove_if([](std::unique_ptr<enemyBullet>& bullet)
		{
			return bullet->isBulletArive() == false;
		});

	for (std::unique_ptr<enemyBullet>& bullet : Bullets)
	{
		bullet->update();
	}

	position = enemyObject->getPosition();
	enemyObject->SetPosition(position);
	enemyObject->Update();

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

void uniteParts::partsArrival()
{
	if (!isAppear)
	{
		return;
	}

	position = enemyObject->getPosition();

	angleThetaRad = angleTheta * (pi / radiannum);
	anglePhiRad = anglePhi * (pi / radiannum);

	position =
	{
		motherPosition->x + partsRadius * sinf(angleThetaRad) * cosf(anglePhiRad),
		motherPosition->y + partsRadius * sinf(angleThetaRad) * sinf(anglePhiRad),
		motherPosition->z + partsRadius * cosf(angleThetaRad)
	};

	enemyObject->SetPosition(position);
	enemyObject->Update();
}

void uniteParts::partsAriveMove()
{
	if (!Isarive || isAppear)
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
		motherPosition->x + partsRadius * sinf(angleThetaRad) * cosf(anglePhiRad),
		motherPosition->y + partsRadius * sinf(angleThetaRad) * sinf(anglePhiRad),
		motherPosition->z + partsRadius * cosf(angleThetaRad)
	};

	enemyObject->SetPosition(position);
	enemyObject->Update();

	//HPが0になったら消滅
	if (HP <= 0)
	{
		Isarive = false;
		fallDownCount = 0;
		angleTheta = 0;
		anglePhi = 0;

		enemyObject->setParent(nullptr);

#pragma region 爆発パーティクル生成
		std::unique_ptr<SingleParticle> newparticle = std::make_unique<SingleParticle>();
		newparticle->generate();
		newparticle->set(maxFallCount - 20, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 0.2f, 10.0f);
		bomParticles.push_back(std::move(newparticle));
#pragma endregion 爆発パーティクル生成
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
	if (Isarive || isAppear)
	{
		return;
	}

	position = enemyObject->getPosition();

	//爆発パーティクル更新
	bomParticles.remove_if([](std::unique_ptr<SingleParticle>& newparticle)
		{
			return newparticle->frame == newparticle->num_frame;
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
			return newparticle->frame == newparticle->num_frame;
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

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet->isArive = false;
	}
}

void uniteParts::partsShotBullet(XMFLOAT3 targetposition)
{
	//弾の発射
	std::unique_ptr<enemyBullet> newBullet = std::make_unique<enemyBullet>();
	newBullet->init();
	newBullet->set(targetposition, this->position);
	Bullets.push_back(std::move(newBullet));
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
	Bullets.clear();

	this->position =
	{
		motherPosition->x + partsRadius * sinf(angleTheta) * cosf(anglePhi),
		motherPosition->y + partsRadius * sinf(angleTheta) * sinf(anglePhi),
		motherPosition->z + partsRadius * cosf(angleTheta)
	};

	enemyObject->SetPosition(this->position);
	enemyObject->Update();

	changePattern(enemyPattern::rampage);
}

void uniteParts::partsDraw3D(directX* directx)
{
	draw3D(directx);
}

void uniteParts::partsDraw2D(directX* directx)
{
	draw2D(directx);

	for (int i = 0; i < HP; i++)
	{
		//partsHitPoint[i]->DrawSprite(directx->cmdList.Get());
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
	enemyObject->SetModel(uniteBossModel.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });
	enemyObject->setColor({ 0.3f,0.3f,0.3f,1 });

	enemyCollision.radius = 2.0f;

	HP = 5;
	uniteBossScale = { 4,4,4 };
	enemyCollision.radius = 9.0f;
	deathRotSpeed = 0.1f;

	bullet = std::make_unique<enemyBullet>();
	bullet->init();

	uniteParts::setStaticData(&position);

	//パーツ初期化＆親オブジェクト設定
	for (int i = 0; i < unitNum; i++)
	{
		std::unique_ptr<uniteParts> newParts = std::make_unique<uniteParts>();
		newParts->partsInit(i);
		partsList.push_back(std::move(newParts));
	}
}

//更新
void uniteBoss::uniteBossUpdata()
{
	enemyObject->Update();

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

	rockTarget->SpriteTransferVertexBuffer();
	rockTarget->SpriteUpdate();

	outScreenIcon[0]->SpriteTransferVertexBuffer();
	outScreenIcon[0]->SpriteUpdate();
	outScreenIcon[1]->SpriteTransferVertexBuffer();
	outScreenIcon[1]->SpriteUpdate();

	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->updataSprite();
	}
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
	enemyObject->Update();

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
		enemyObject->Update();
		isStop = false;
		Isarive = true;
		isAppear = false;
		isArmor = true;
		playerPointer->isStop = false;
		playerPointer->isInvisible = -1;
		isSelectPattern = true;

		for (std::unique_ptr<uniteParts>& parts : partsList)
		{
			parts->partsUpdata();
			parts->isStop = false;
			parts->Isarive = true;
			parts->isAppear = false;
			parts->angleSpeed = 0.01f;
		}

		Object3d_FBX::SetCamera(playerPointer->followCamera);
	}
}

//生存時処理
void uniteBoss::uniteBossAriveMove()
{
	if (!Isarive || isAppear)
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
		Isarive = false;
		fallDownCount = 0;
		deathRotSpeed = 0.5f;
		CameraAngleSpeed = 0.0f;
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
	if (Isarive || isAppear)
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

	CameraAngleSpeed += 0.5f;

	uniteBossCamera->SetEye(setvec);
	uniteBossCamera->SetTarget(position);
	uniteBossCamera->Update();

	Object3d_FBX::SetCamera(uniteBossCamera);
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

	//演出が終わったら
	if (fallDownCount >= maxFallCount * 6)
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

//セット
void uniteBoss::uniteBossSet()
{
	arrivalStartPos = { 0,80,0 };
	arrivalEndPos = { 0,5,0 };
	position = arrivalStartPos;
	isTargetSet = false;
	chaseCount = 0;
	waitCount = 0;
	isChase = false;
	isWait = true;
	isDraw = true;

	nextBulletTime = 0;
	bulletCount = 0;
	maxBulletCount = 20;
	isRampageWait = true;
	rampageWaitCount = 0;
	Bullets.clear();

	HP = resetHitPoint;
	arrivalTime = 300;
	enemyObject->SetScale(uniteBossScale);
	enemyObject->SetPosition({ 0,0,0 });
	enemyObject->Update();

	//演出用カメラをセット
	uniteBossCamera = new Camera;
	uniteBossCamera->SetEye({ 0,0,position.z - 35 });
	uniteBossCamera->SetTarget(position);
	Object3d_FBX::SetCamera(uniteBossCamera);

	std::list<XMFLOAT2>::iterator itr = defaultPartsAngle.begin();

	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsSet({ 0,0,0 }, itr->x, itr->y);
		parts->angleSpeed = 0.0f;
		itr++;
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

	if (!playerPointer->isArive)
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
	}

	//追跡
	if (isChase)
	{
		//追尾カウント加算
		chaseCount++;
		enemySpeed = 0.45f;
		if (chaseCount >= 1)
		{
			isChase = false;
			chaseCount = 0;
			isWait = true;
			chargeAttackCount++;
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
		if (waitCount >= 100)
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

	if (!playerPointer->isArive)
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

	if (nextBulletTime % 5 == 0)
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

	if (!playerPointer->isArive)
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


	if (!bullet->isBulletArive() && isInRange)
	{
		nextShotTime++;
	}

	if (nextShotTime >= 40 && bullet->isBulletArive() == false)
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

	bullet->update();
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
	draw2D(directx);

	for (int i = 0; i < HP; i++)
	{
		//motherHitPoint[i]->DrawSprite(directx->cmdList.Get());
	}

	//パーツの描画
	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsDraw2D(directx);
	}
}
#pragma endregion 本体
