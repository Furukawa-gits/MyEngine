#include"Enemy.h"
#include<random>

std::unique_ptr<Model> Enemy::enemyModelS = std::make_unique<Model>();
XMFLOAT3 Enemy::playerPosition = { 0,0,0 };
XMFLOAT3 Enemy::playerFront = { 0,0,0 };
const float Enemy::forPlayer = 200.0f;

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

void Enemy::staticUpdata(XMFLOAT3 playerpos, XMFLOAT3 playerfront)
{
	playerPosition = playerpos;
	playerFront = playerfront;
}

void Enemy::staticDestroy()
{
}

void Enemy::init(enemyPattern pattern)
{
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
	enemyObject->SetModel(enemyModelS.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });

	enemyCollision.radius = 2.0f;

	enemyMovePattern = pattern;

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet = std::make_unique<enemyBullet>();
		bullet->init();
	}
}

void Enemy::set(XMFLOAT3 pos)
{
	position = pos;
	startPosition = pos;
	enemyObject->SetPosition(pos);
	Isarive = false;
	isTargetSet = false;
	chaseCount = 0;
	waitCount = 0;
	isChase = false;
	isWait = true;
	isDraw = true;
	enemyArrivalTime = 100;
	enemyArrivaCount = 0;
	arrivalEase.set(easingType::easeOut, easingPattern::Quadratic, enemyArrivalTime, 500, 0);

	isAppear = true;
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

void Enemy::tutorial()
{
	if (enemyMovePattern != enemyPattern::tutorial)
	{
		return;
	}

	enemyObject->Update();
}

void Enemy::chase()
{
	if (enemyMovePattern != enemyPattern::chase)
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
	if (!bullet->isBulletArive() && isInRange)
	{
		shotCount++;
	}

	//一定カウントごとに射撃フラグを立てる
	if (shotCount >= 10 && bullet->isBulletArive() == false)
	{
		isShot = true;
		shotCount = 0;
	}
	else
	{
		isShot = false;
	}

	//弾を撃つ
	if (isShot)
	{
		bullet->set(playerPosition, this->position);
		isShot = false;
	}

	bullet->update();
}

void Enemy::homing()
{
	if (enemyMovePattern != enemyPattern::homing)
	{
		return;
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

	if (!bullet->isBulletArive() && isInRange)
	{
		shotCount++;
	}

	if (shotCount >= 10 && bullet->isBulletArive() == false)
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
		bullet->set(playerPosition, this->position);
		isShot = false;
	}

	bullet->update();

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

}

void Enemy::update()
{
	if (!isDraw)
	{
		return;
	}

	arrival();

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
	float cross = playerToEnemy.x * playerFront.x + playerToEnemy.y * playerFront.y + playerToEnemy.z * playerFront.z;

	float vecLen = sqrtf((powf(playerToEnemy.x, 2) + powf(playerToEnemy.y, 2) + powf(playerToEnemy.z, 2)) * (powf(playerFront.x, 2) + powf(playerFront.y, 2) + powf(playerFront.z, 2)));

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

	//敵が生存
	ariveMove();

	//敵が撃墜
	deathMove();

	return;
}

void Enemy::updataSprite()
{
	XMFLOAT2 targetPos = enemyObject->worldToScleen();

	rockTarget->rotation += 1.5f;

	//ターゲットアイコンと画面外アイコンの処理
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

	if (isOutScreen)
	{
		targetPos.y = 720 - outScreenIcon[0]->size.y / 2;
	}

	rockTarget->position = { targetPos.x,targetPos.y,0 };
	outScreenIcon[0]->position = { targetPos.x,targetPos.y,0 };
	outScreenIcon[1]->position = { targetPos.x,targetPos.y,0 };

	rockTarget->SpriteTransferVertexBuffer();
	rockTarget->SpriteUpdate();

	outScreenIcon[0]->SpriteTransferVertexBuffer();
	outScreenIcon[0]->SpriteUpdate();
	outScreenIcon[1]->SpriteTransferVertexBuffer();
	outScreenIcon[1]->SpriteUpdate();
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
	enemyObject->Update();

	if (!arrivalEase.getIsActive())
	{
		enemyObject->SetRotation({ 0,0,0 });
		enemyObject->SetPosition(position);
		enemyObject->SetScale({ 1,1,1 });
		enemyObject->Update();
		enemyArrivaCount = 0;
		isStop = false;
		Isarive = true;
		isAppear = false;
	}
}

void Enemy::ariveMove()
{
	if (!Isarive || isAppear)
	{
		return;
	}

	tutorial();

	chase();

	shot();

	homing();

	//HPが0になったら消滅
	if (HP <= 0)
	{
		Isarive = false;
		fallDownCount = 0;

#pragma region 爆発パーティクル生成
		std::unique_ptr<SingleParticle> newparticle = std::make_unique<SingleParticle>();
		newparticle->generate();
		newparticle->set(maxFallCount - 20, position, { 0,0,0 }, { 0,0,0 }, 0.2f, 10.0f);
		bomParticles.push_back(std::move(newparticle));
#pragma endregion 爆発パーティクル生成
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
	if (Isarive || isAppear)
	{
		return;
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
		newparticle->set((maxFallCount - fallDownCount) + 10, position, { 0,0,0 }, { 0,0,0 }, 3.5, 0.5);

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

	enemyObject->Draw(directx->cmdList.Get());

	if (enemyMovePattern == enemyPattern::shot || enemyMovePattern == enemyPattern::homing)
	{
		if (Isarive)
		{
			bullet->draw(directx);
		}
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
	if (isFar || !Isarive)
	{
		return;
	}

	if (isOutScreen)
	{
		outScreenIcon[0]->DrawSprite(directx->cmdList.Get());
		outScreenIcon[1]->DrawSprite(directx->cmdList.Get());
	}

	if (isTargetSet)
	{
		rockTarget->DrawSprite(directx->cmdList.Get());
	}
}
#pragma endregion

#pragma region 敵の弾
std::unique_ptr<Model> enemyBullet::buletModelS = std::make_unique<Model>();

enemyBullet::enemyBullet()
{
}

enemyBullet::~enemyBullet()
{
	delete(bulletObject);
}

void enemyBullet::staticInit()
{
	buletModelS.reset(FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01"));
}

void enemyBullet::staticDestroy()
{
}

void enemyBullet::init()
{
	bulletObject = new Object3d_FBX;
	bulletObject->Initialize();
	bulletObject->SetModel(buletModelS.get());
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