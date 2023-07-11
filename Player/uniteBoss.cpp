#include "uniteBoss.h"
#include <random>

//static�ϐ�(�p�[�c)
std::unique_ptr<Model> uniteParts::partsModel = std::make_unique<Model>();
XMFLOAT3* uniteParts::motherPosition = nullptr;
const float uniteParts::partsRadius = 15.0f;

//static�ϐ�(�{��)
std::unique_ptr<Model> uniteBoss::uniteBossModel = std::make_unique<Model>();
Player* uniteBoss::playerPointer = nullptr;

#pragma region �p�[�c
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
	//�{�̂̍��W���Z�b�g
	motherPosition = motherposition;

	//���f���ǂݍ���
	partsModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_02"));
}

void uniteParts::partsUpdata()
{
	//�o�ꉉ�o
	partsArrival();

	//�e�̍X�V
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

	//�v���C���[�Ƃ̈ʒu�֌W���v�Z
	XMFLOAT3 playerToEnemy =
	{
		position.x - playerPosition.x,
		position.y - playerPosition.y,
		position.z - playerPosition.z,
	};

	float length = sqrtf(powf(playerToEnemy.x, 2) + powf(playerToEnemy.y, 2) + powf(playerToEnemy.z, 2));

	//�v���C���[�Ƌ��������ꂷ���Ă��邩
	if (length >= forPlayer)
	{
		isFar = true;
	}
	else
	{
		isFar = false;
	}

	//�x�N�g���̓��ς���p�x�����߂āA���b�N�I���͈̔͂��i��
	float dot = playerToEnemy.x * playerFront.x + playerToEnemy.y * playerFront.y + playerToEnemy.z * playerFront.z;

	float vecLen = sqrtf((powf(playerToEnemy.x, 2) + powf(playerToEnemy.y, 2) + powf(playerToEnemy.z, 2)) * (powf(playerFront.x, 2) + powf(playerFront.y, 2) + powf(playerFront.z, 2)));

	toPlayerAngle = acosf(dot / vecLen) * (180.0f / (float)M_PI);

	if (toPlayerAngle > 90)
	{
		//��ʊO(�����̈ʒu���v���C���[�̌����ƒ��p�ȏ�)�ɂ���Ƃ���ʊO�t���O�𗧂Ă�
		isOutScreen = true;
	}
	else
	{
		isOutScreen = false;
	}

	//����������
	partsAriveMove();

	//���ĉ��o
	partsDeathMove();
}

void uniteParts::partsSpriteUpdata()
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

	//�{�̂̎������]����
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

	//�{�̂̎������]����
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

	//HP��0�ɂȂ��������
	if (HP <= 0)
	{
		isAlive = false;
		fallDownCount = 0;
		angleTheta = 0;
		anglePhi = 0;

		enemyObject->setParent(nullptr);

#pragma region �����p�[�e�B�N������
		SingleParticle newp;
		newp.generate();
		newp.set(maxFallCount - 20, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 0.2f, 10.0f);
		particleManagerOnTime::addParticle(newp, "bomb.png");
#pragma endregion �����p�[�e�B�N������

		//�e������
		normalBullets.clear();
	}

	//�����蔻��X�V
	enemyObject->SetPosition(position);
	enemyCollision.center =
	{
		enemyObject->getPosition().x,
		enemyObject->getPosition().y,
		enemyObject->getPosition().z,1.0f
	};

	//�X�v���C�g�X�V
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

#pragma region �����p�[�e�B�N��
	if (fallDownCount % 15 == 0)
	{
		SingleParticle newp;
		newp.generate();
		newp.set((maxFallCount - fallDownCount) + 10, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 3.5, 0.5);
		particleManagerOnTime::addParticle(newp, "smoke.png");
	}
#pragma endregion �����p�[�e�B�N��

	//������������
	if (fallDownCount >= maxFallCount)
	{
		isDraw = false;
		fallDownCount = 0;
	}
}

void uniteParts::partsShotBullet(XMFLOAT3 targetposition)
{
	//�e�̔���
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
#pragma endregion �p�[�c

#pragma region �{��
uniteBoss::uniteBoss()
{
}

uniteBoss::~uniteBoss()
{
}

//�ÓI������
void uniteBoss::uniteBossStaticInit(Player* player)
{
	uniteBossModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("boss"));

	playerPointer = player;
}

//������
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

//�X�V
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

	//�~�j�}�b�v�A�C�R���X�V
	XMFLOAT3 uniteBossPosition = position;

	//�ォ�猩���ʒu�Ȃ̂� X�EZ���W
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

//�X�v���C�g�X�V
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

	//���x���[�^�[�X�V
	//y���W�ɕ␳��������position�ɑ��
	XMFLOAT3 position = enemyObject->getPosition();
	float enemyH = position.y - -180;

	//�X�v���C�g�̍��W�n�ɕύX�����[�^�[�̏オ���𒲐�
	enemyH = -(enemyH * 0.3333f);

	//���ۂɕ\��������W�ɕύX
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

//�o�ꉉ�o
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

//����������
void uniteBoss::uniteBossAriveMove()
{
	if (!isAlive || isAppear)
	{
		return;
	}

	//�s���p�^�[���̒��I
	selectMovePattern();

	//�ːi
	uniteBossChargeAttack();

	//���@�_���e
	uniteBossShotPlayerTarget();

	//�e����
	uniteBossRampage();

	//�p�[�c�̍X�V
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

	//�S�Ẵp�[�c���j�󂳂ꂽ���ǂ���
	if (partsList.size() <= 0)
	{
		isAllPartsBreak = true;
		isArmor = false;
	}

	//HP��0�ɂȂ��������
	if (HP <= 0)
	{
		isAlive = false;
		fallDownCount = 0;
		deathRotSpeed = 0.5f;
		CameraAngleSpeed = 0.0f;

		//�e������
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

//���ď���
void uniteBoss::uniteBossDeathMove()
{
	if (isAlive || isAppear)
	{
		return;
	}

	playerPointer->isStop = true;
	playerPointer->isInvisible = 1;

	position = enemyObject->getPosition();

	//�{�X�̎��͂��J��������]����
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

	//���ĉ��o�̃J�E���g
	fallDownCount++;

	float scale = 1.0f - ((float)fallDownCount / ((float)maxFallCount * 7));

	rot.x += deathRotSpeed;
	rot.y += deathRotSpeed;
	rot.z += deathRotSpeed;
	enemyObject->setRotMatrix(rot.x, rot.y, rot.z);
	enemyObject->SetScale({ uniteBossScale.x * scale,uniteBossScale.y * scale,uniteBossScale.z * scale });

	//���Ԋu�ŃG�t�F�N�g
	if (fallDownCount % 20 == 0)
	{
#pragma region �p�[�e�B�N������
		//�����ʒu�������_���Ŏw��
		float randX = (float)(rand() % 40) - 20;
		float randY = (float)(rand() % 40) - 20;
		float randZ = (float)(rand() % 40) - 20;
		XMFLOAT3 startPos =
		{
			enemyObject->getPosition().x + randX,
			enemyObject->getPosition().y + randY,
			enemyObject->getPosition().z + randZ
		};

		//�����p�[�e�B�N��
		SingleParticle newbp;
		newbp.generate();
		newbp.set(50, startPos, { 0,0,0 }, { 0,0,0 }, 0.2, 9.0);
		particleManagerOnTime::addParticle(newbp, "bomb.png");

		//�����p�[�e�B�N��
		SingleParticle newsp;
		newsp.generate();
		newsp.set(50, startPos, { 0,0,0 }, { 0,0,0 }, 0.2, 5.0);
		particleManagerOnTime::addParticle(newsp, "smoke.png");
#pragma endregion �p�[�e�B�N������
	}

	//���o���I�������
	if (fallDownCount >= maxFallCount * 5)
	{
		isDraw = false;
		fallDownCount = 0;
	}
}

//�Z�b�g
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

	//���o�p�J�������Z�b�g
	uniteBossCamera = new Camera;
	uniteBossCamera->SetEye({ 0,0,position.z - 35 });
	uniteBossCamera->SetTarget(position);
	object3dFBX::SetCamera(uniteBossCamera);

	partsList.clear();

	//�p�[�c���������e�I�u�W�F�N�g�ݒ�
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

//�s���p�^�[�����I
void uniteBoss::selectMovePattern()
{
	if (!isSelectPattern)
	{
		return;
	}

	uniteBossNowPattern = (uniteBossPattern)(rand() % 3);

	isSelectPattern = false;
}

//�ːi
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

	//�ǐ�
	if (isChase)
	{
		//�ǔ��J�E���g���Z
		chaseCount++;
		enemySpeed = 1.5f;
		if (chaseCount >= 1)
		{
			isChase = false;
			chaseCount = 0;
			isWait = true;
		}
	}

	//����
	if (isWait)
	{
		//�ҋ@�J�E���g���Z
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

//����
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
		//�p�[�c����e�𔭎�
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

//���@�_���e
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

	//�{�̂���v���C���[�ւ̋������v�Z
	float length = sqrtf(powf(startToTarget.x, 2) + powf(startToTarget.y, 2) + powf(startToTarget.z, 2));

	//�˒��͈͓������f
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

	//�ˌ�
	if (isShot)
	{
		//�p�[�c����e�𔭎�
		for (std::unique_ptr<uniteParts>& parts : partsList)
		{
			parts->partsShotBullet(pPos);
		}
		shotCount++;
		isShot = false;
	}
}

//�`��_3D
void uniteBoss::uniteBossDraw3d(directX* directx)
{
	draw3D(directx);

	//�p�[�c�̕`��
	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsDraw3D(directx);
	}
}

//�`��_2D
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

	//�p�[�c�̕`��
	for (std::unique_ptr<uniteParts>& parts : partsList)
	{
		parts->partsDraw2D(directx);
	}
}
#pragma endregion �{��
