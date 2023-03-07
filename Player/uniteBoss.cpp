#include "uniteBoss.h"

//static�ϐ�(�p�[�c)
XMFLOAT3* uniteParts::motherPosition = nullptr;
const float uniteParts::partsRadius = 5.0f;

//static�ϐ�(�{��)
std::unique_ptr<Model> uniteBoss::uniteBossModel = std::make_unique<Model>();
Player* uniteBoss::playerPointer = nullptr;

#pragma region �p�[�c
void uniteParts::partsInit(int ID)
{
	//���N���X�̏�����
	init(enemyPattern::shot);
	enemyObject->setColor({ 1.0f,1.0f,1.0f,1 });
	isThisBoss = true;
}

void uniteParts::setStaticData(XMFLOAT3* motherposition)
{
	//�{�̂̍��W���Z�b�g
	motherPosition = motherposition;
}

void uniteParts::partsUpdata()
{
	//�e�̍X�V
	Bullets.remove_if([](std::unique_ptr<enemyBullet>& bullet)
		{
			return bullet->isBulletArive() == false;
		});

	for (std::unique_ptr<enemyBullet>& bullet : Bullets)
	{
		bullet->update();
	}

	//�{�̂̎������]����
	anglePhi += angleSpeed;
	angleTheta += angleSpeed;

	partsPosition =
	{
		partsRadius * sinf(angleTheta * (M_PI / 180.0f)) * cosf(anglePhi * (M_PI / 180.0f)),
		partsRadius * sinf(angleTheta * (M_PI / 180.0f)) * sinf(anglePhi * (M_PI / 180.0f)),
		partsRadius * cosf(angleTheta * (M_PI / 180.0f)),
	};

	enemyObject->SetPosition(partsPosition);
	enemyObject->Update();
}

void uniteParts::partsShotBullet(XMFLOAT3 targetposition)
{
	//�e�̔���
	std::unique_ptr<enemyBullet> newBullet = std::make_unique<enemyBullet>();
	newBullet->init();
	newBullet->set(targetposition, this->position);
	Bullets.push_back(std::move(newBullet));
}

void uniteParts::partsSet(XMFLOAT3 position, float theta, float phi)
{
	partsPosition = position;
	anglePhi = phi;
	angleTheta = theta;
	HP = 6;
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
		partsHitPoint[i]->DrawSprite(directx->cmdList.Get());
	}
}
#pragma endregion �p�[�c

#pragma region �{��
uniteBoss::uniteBoss()
{
}

uniteBoss::~uniteBoss()
{
}

void uniteBoss::uniteBossStaticInit(Player* player)
{
	uniteBossModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("boss"));

	playerPointer = player;
}

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

	uniteParts::setStaticData(&position);

	//�p�[�c���������e�I�u�W�F�N�g�ݒ�
	for (int i = 0; i < unitNum; i++)
	{
		std::unique_ptr<uniteParts> newParts = std::make_unique<uniteParts>();
		newParts->partsInit(i);
		partsList.push_back(std::move(newParts));
	}
}

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

	//�x�N�g���̓��ς���p�x�����߂āA���b�N�I���͈̔͂��i��
	float cross = toPlayer.x * playerFront.x + toPlayer.y * playerFront.y + toPlayer.z * playerFront.z;

	float vecLen = sqrtf((powf(toPlayer.x, 2) + powf(toPlayer.y, 2) + powf(toPlayer.z, 2)) * (powf(playerFront.x, 2) + powf(playerFront.y, 2) + powf(playerFront.z, 2)));

	toPlayerAngle = acosf(cross / vecLen) * (180.0f / (float)M_PI);

	if (toPlayerAngle > 90)
	{
		//��ʊO(�����̈ʒu���v���C���[�̌����ƒ��p�ȏ�)�ɂ���Ƃ���ʊO�t���O�𗧂Ă�
		isOutScreen = true;
	}
	else
	{
		isOutScreen = false;
	}

	uniteBossAriveMove();

	uniteBossDeathMove();
}

void uniteBoss::uniteBossSpriteUpdata()
{
	XMFLOAT2 targetPos = enemyObject->worldToScleen();
	XMFLOAT2 targetPosOutScreen = enemyObject->worldToScleen();

	rockTarget->rotation += 1.5f;

	//�^�[�Q�b�g�A�C�R��
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

	//��ʊO�A�C�R��
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

void uniteBoss::uniteBossArrival()
{
	if (!isAppear)
	{
		return;
	}

	isAppear = true;
}

void uniteBoss::uniteBossAriveMove()
{
	if (!Isarive || isAppear)
	{
		return;
	}

	uniteBossChargeAttack();

	uniteBossShotPlayerTarget();

	uniteBossRampage();

	//�p�[�c�̍X�V
	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsUpdata();
	}

	//HP��0�ɂȂ��������
	if (HP <= 0)
	{
		Isarive = false;
		fallDownCount = 0;
		deathRotSpeed = 0.5f;
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

void uniteBoss::uniteBossDeathMove()
{
}

void uniteBoss::uniteBossSet()
{
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
	enemyObject->SetScale(uniteBossScale);
	enemyObject->SetPosition({ 0,0,0 });
	enemyObject->Update();

	//���o�p�J�������Z�b�g
	bossCamera = new Camera;
	bossCamera->SetEye({ 0,0,0 });
	bossCamera->SetTarget({ 0,0,0 });
	Object3d_FBX::SetCamera(bossCamera);

	isAppear = true;
}

void uniteBoss::selectMovePattern()
{
	if (!isSelectPattern)
	{
		return;
	}

	uniteBossNowPattern = (uniteBossPattern)(rand() % 3);

	isSelectPattern = false;
}

void uniteBoss::uniteBossChargeAttack()
{
	if (uniteBossNowPattern != uniteBossPattern::chargeAttack)
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

	//�ǐ�
	if (isChase)
	{
		//�ǔ��J�E���g���Z
		chaseCount++;
		enemySpeed = 0.35f;
		if (chaseCount >= 1)
		{
			isChase = false;
			chaseCount = 0;
			isWait = true;
		}
	}

	//�ҋ@
	if (isWait)
	{
		//�ҋ@�J�E���g���Z
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

void uniteBoss::uniteBossRampage()
{
	if (uniteBossNowPattern != uniteBossPattern::rampageBullet)
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

void uniteBoss::uniteBossShotPlayerTarget()
{
	if (uniteBossNowPattern != uniteBossPattern::shotPlayerTarget)
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
		bullet->set(pPos, this->position);
		isShot = false;
	}

	bullet->update();
}

void uniteBoss::uniteBossDraw3d(directX* directx)
{
	draw3D(directx);

	//�p�[�c�̕`��
	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsDraw3D(directx);
	}
}

void uniteBoss::uniteBossDraw2d(directX* directx)
{
	draw2D(directx);

	for (int i = 0; i < HP; i++)
	{
		motherHitPoint[i]->DrawSprite(directx->cmdList.Get());
	}

	//�p�[�c�̕`��
	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsDraw2D(directx);
	}
}
#pragma endregion �{��
