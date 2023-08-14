#include"ChaseEnemy.h"

ChaseEnemy::ChaseEnemy()
{
}

ChaseEnemy::~ChaseEnemy()
{
}

void ChaseEnemy::init(bool isboss)
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

	bodyColor = { 0.3f,1,0.3f,1 };

	enemyObject = std::make_unique<object3dFBX>();
	enemyObject->initialize();
	enemyObject->SetModel(staticEnemyModel.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });
	enemyObject->setColor(bodyColor);

	enemyCollision.radius = 2.0f;

	myEnemyType = enemyType::chase;

	isThisBoss = isboss;

	if (isThisBoss)
	{
		bossHitPointGauge = std::make_unique<SingleSprite>();
		bossHitPointGauge->anchorpoint = { 0.5f,0.5f };
		bossHitPointGauge->size = { 50,20 };
		bossHitPointGauge->position = { 640,40,0 };
		bossHitPointGauge->generateSprite("bossHPGauge.png");

		HP = 5;
		bossbaseScale = { 5,5,5 };
		enemyCollision.radius = 9.0f;
		deathRotSpeed = 0.1f;
	}
}

void ChaseEnemy::set(XMFLOAT3 pos)
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
	enemyArrivalTime = 0;

	//�{�X�������ꍇ
	if (isThisBoss)
	{
		bossbaseScale = { 5,5,5 };
		enemyCollision.radius = 9.0f;
		deathRotSpeed = 0.1f;
		enemyArrivalMaxTime = 300;
		arrivalEase.set(easingType::easeOut, easingPattern::Quadratic, enemyArrivalMaxTime, 1000, 0);
		arrivalMaxScale = { 5,5,5 };
		//���o�p�J�������Z�b�g
		bossCamera->SetEye({ pos.x - 10,pos.y - 10,pos.z + 10 });
		bossCamera->SetTarget({ pos.x + 5,pos.y + 5,pos.z - 5 });
		object3dFBX::SetCamera(bossCamera);
	}
	else
	{
		enemyArrivalMaxTime = 100;
		arrivalEase.set(easingType::easeOut, easingPattern::Quadratic, enemyArrivalMaxTime, 500, 0);
		arrivalMaxScale = { 1,1,1 };
	}

	isAppear = true;
}

void ChaseEnemy::updata()
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

void ChaseEnemy::arrival()
{
	if (!isAppear)
	{
		return;
	}

	isDraw = true;

	enemyArrivalTime++;

	arrivalScale.x = (float)enemyArrivalTime / (float)enemyArrivalMaxTime;
	arrivalScale.y = (float)enemyArrivalTime / (float)enemyArrivalMaxTime;
	arrivalScale.z = (float)enemyArrivalTime / (float)enemyArrivalMaxTime;

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
		enemyArrivalTime = 0;
		isStop = false;
		isAlive = true;
		isAppear = false;
	}
}

void ChaseEnemy::ariveMove()
{
	if (!isAlive || isAppear)
	{
		return;
	}

	chase();

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

void ChaseEnemy::deathMove()
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

void ChaseEnemy::deathMoveBoss()
{
	if (isAlive || isAppear)
	{
		return;
	}

	*playerIsStop = true;
	*playerIsInvisible = 1;

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

	//���ĉ��o�̃J�E���g
	fallDownCount++;

	scale = 1.0f - ((float)fallDownCount / ((float)maxFallCount * 3));

	rot.x += deathRotSpeed;
	rot.y += deathRotSpeed;
	rot.z += deathRotSpeed;
	enemyObject->setRotMatrix(rot.x, rot.y, rot.z);
	enemyObject->SetScale({
		arrivalMaxScale.x * scale,
		arrivalMaxScale.y * scale,
		arrivalMaxScale.z * scale });

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

	//������������
	if (fallDownCount >= maxFallCount * 3)
	{
		isDraw = false;
		fallDownCount = 0;
	}
}

void ChaseEnemy::updataSprite()
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

void ChaseEnemy::draw3D()
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
}

void ChaseEnemy::draw2D()
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

void ChaseEnemy::drawMiniMapIcon()
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

void ChaseEnemy::chase()
{
	if (!playerIsAlive)
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