#include "RampageEnemy.h"

RampageEnemy::RampageEnemy()
{
}

RampageEnemy::~RampageEnemy()
{
}

void RampageEnemy::init()
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

	myEnemyType = enemyType::rampage;

	bodyColor = { 1,0.2f,0.2f,1 };
}

void RampageEnemy::set(XMFLOAT3 pos)
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
	nextBulletTime = 0;
	bulletCount = 0;
	isRampageWait = true;
	rampageWaitCount = 0;
	normalBullets.clear();

	isAppear = true;
}

void RampageEnemy::updata()
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

	if (toPlayerAngle > 90 && !isAppear)
	{
		//��ʊO(�����̈ʒu���v���C���[�̌����ƒ��p�ȏ�)�ɂ���Ƃ���ʊO�t���O�𗧂Ă�
		isOutScreen = true;
	}
	else
	{
		isOutScreen = false;
	}

	//�G������
	ariveMove();

	//�G������
	deathMove();

	enemyObject->SetScale({ scale ,scale ,scale });
	enemyObject->SetPosition(position);

	return;
}

void RampageEnemy::arrival()
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

void RampageEnemy::ariveMove()
{
	if (!isAlive || isAppear)
	{
		return;
	}

	rampage();

	//HP��0�ɂȂ��������
	if (HP <= 0)
	{
		isAlive = false;
		fallDownCount = 0;

#pragma region �����p�[�e�B�N������
		SingleParticle newp;
		newp.generate();
		newp.set(maxFallCount - 20, enemyObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 0.2f, 10.0f);
		particleManagerOnTime::addParticle(newp, "bomb.png");
#pragma endregion �����p�[�e�B�N������

#pragma region �j�Ѓp�[�e�B�N������
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
#pragma endregion �j�Ѓp�[�e�B�N������
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

void RampageEnemy::deathMove()
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

void RampageEnemy::updataSprite()
{
	XMFLOAT2 targetPos = enemyObject->worldToScleen();

	rockTarget->rotation += 2.5f;

	//���b�N�I���A�C�R���̃T�C�Y���傫����΃f�t�H���g�ɋ߂Â��Ă���
	if (rockTarget->size.x > defaultRockIconSize.x)
	{
		rockTarget->size.x -= decreaseSize;
		rockTarget->size.y -= decreaseSize;
	}
	else
	{
		rockTarget->size = defaultRockIconSize;
	}

	//�e�A�C�R������ʊO�ɍs���Ȃ��悤�ɂ���
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

	//�v���C���[�̎��E�O�ɍs������A��ʂ̉��ɃA�C�R�����ړ�������
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

	enemyHeight->position = { miniMapPosition.x + 120,enemyH,0 };
	miniMapEnemy->position = { miniMapPosition.x + 140,enemyH - 2,0 };

	miniMapEnemy->spriteUpdata();
	enemyHeight->spriteUpdata();
}

void RampageEnemy::draw3D()
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

void RampageEnemy::draw2D()
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

void RampageEnemy::drawMiniMapIcon()
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

void RampageEnemy::rampage()
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
		newBullet->init({ 1,0,0,1 });

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
