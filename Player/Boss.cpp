#include "Boss.h"

std::unique_ptr<Model> Boss::bossModel = std::make_unique<Model>();

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::staticInitBoss()
{
	bossModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("boss"));
}

void Boss::bossInit()
{
	isThisBoss = true;

	Isarive = false;
	isDraw = false;

	rockTarget = std::make_unique<SingleSprite>();
	rockTarget->anchorpoint = { 0.5f,0.5f };
	rockTarget->size = { 70,70 };
	rockTarget->GenerateSprite("Rock_on.png");

	enemyObject = new Object3d_FBX;
	enemyObject->Initialize();
	enemyObject->SetModel(bossModel.get());
	enemyObject->SetScale({ 1.0f,1.0f,1.0f });

	enemyCollision.radius = 2.0f;

	enemyMovePattern = enemyPattern::shot;

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet = std::make_unique<enemyBullet>();
		bullet->init();
	}

	HP = 5;
	bossbaseScale = { 5,5,5 };
	enemyObject->setColor({ 0.3f,0.3f,0.3f,1 });
	enemyCollision.radius = 9.0f;
	deathRotSpeed = 0.1f;
}

void Boss::bossUpdate(Player* player)
{
	enemyObject->Update();

	bossArrival(player);

	playerPointer = player;

	XMFLOAT3 playerPos = playerPointer->playerObject->getPosition();

	if (isStop)
	{
		return;
	}

	XMFLOAT3 toPlayer =
	{
		playerPos.x - position.x,
		playerPos.y - position.y,
		playerPos.z - position.z,
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

	bossAriveMove();

	bossDeathMove();
}

void Boss::bossSet(XMFLOAT3 pos)
{
	position = pos;
	startPosition = pos;
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
	HP = resetHitPoint;
	arrivalTime = 300;
	bossRotEase.set(easingType::easeOut, easingPattern::Quadratic, arrivalTime, 1000, 0);
	bossScale = { 0,0,0 };
	enemyObject->SetScale(bossScale);
	enemyObject->SetPosition(pos);
	enemyObject->Update();

	//���o�p�J�������Z�b�g
	bossCamera = new Camera;
	bossCamera->SetEye({ pos.x - 10,pos.y - 10,pos.z + 10 });
	bossCamera->SetTarget({ pos.x + 5,pos.y + 5,pos.z - 5 });
	Object3d_FBX::SetCamera(bossCamera);

	isAppear = true;
}

void Boss::bossReSet()
{
	reSet();
	HP = resetHitPoint;
	isDraw = false;
}

void Boss::bossArrival(Player* player)
{
	if (!isAppear)
	{
		return;
	}

	player->isStop = true;
	player->isInvisible = 1;

	isStop = true;

	float rot = bossRotEase.easing();

	bossScale.x += bossbaseScale.x / arrivalTime;
	bossScale.y += bossbaseScale.y / arrivalTime;
	bossScale.z += bossbaseScale.z / arrivalTime;

	XMMATRIX matrot = XMMatrixIdentity();
	matrot *= XMMatrixRotationZ(XMConvertToRadians(rot));
	matrot *= XMMatrixRotationX(XMConvertToRadians(rot));
	matrot *= XMMatrixRotationY(XMConvertToRadians(rot));

	enemyObject->setRotMatrix(matrot);
	enemyObject->SetScale(bossScale);
	enemyObject->Update();

	if (!bossRotEase.getIsActive())
	{
		enemyObject->SetRotation({ 0,0,0 });
		enemyObject->SetPosition(position);
		enemyObject->Update();
		isStop = false;
		Isarive = true;
		isAppear = false;
		player->isStop = false;
		player->isInvisible = -1;
		Object3d_FBX::SetCamera(player->followCamera);
	}
}

void Boss::bossAriveMove()
{
	if (!Isarive || isAppear)
	{
		return;
	}

	bossChase();

	bossShot();

	//HP��0�ɂȂ��������
	if (HP <= 0)
	{
		Isarive = false;
		fallDownCount = 0;
		deathRotSpeed = 0.5f;
	}

	if (isTargetSet)
	{
		rockTarget->rotation += 1.5f;
		XMFLOAT2 screenPos = enemyObject->worldToScleen();
		rockTarget->position = { screenPos.x,screenPos.y,0 };
	}

	rockTarget->SpriteTransferVertexBuffer();
	rockTarget->SpriteUpdate();

	enemyObject->SetPosition(position);
	enemyCollision.center =
	{
		enemyObject->getPosition().x,
		enemyObject->getPosition().y,
		enemyObject->getPosition().z,1.0f
	};
}

void Boss::bossDeathMove()
{
	if (Isarive || isAppear)
	{
		return;
	}

	playerPointer->isStop = true;
	playerPointer->isInvisible = 1;

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

	Object3d_FBX::SetCamera(bossCamera);
	SingleParticle::setCamera(bossCamera);

	//���ĉ��o�̃J�E���g
	fallDownCount++;

	rot.x += deathRotSpeed;
	rot.y += deathRotSpeed;
	rot.z += deathRotSpeed;
	enemyObject->setRotMatrix(rot.x, rot.y, rot.z);

	//���Ԋu�ŃG�t�F�N�g
	if (fallDownCount % 20 == 0)
	{
#pragma region �p�[�e�B�N������
		//�����ʒu�����̕\�ʏ�Ŏw��
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

		//�����p�[�e�B�N��
		std::unique_ptr<SingleParticle> newBomparticle = std::make_unique<SingleParticle>();
		newBomparticle->generate();
		newBomparticle->set(30, startPos, { 0,0,0 }, { 0,0,0 }, 0.2, 7.0);
		bomParticles.push_back(std::move(newBomparticle));

		//�����p�[�e�B�N��
		std::unique_ptr<SingleParticle> newSmokeparticle = std::make_unique<SingleParticle>();
		newSmokeparticle->generate();
		newSmokeparticle->set(30, startPos, { 0,0,0 }, { 0,0,0 }, 0.2, 5.0);
		smokeParticles.push_back(std::move(newSmokeparticle));
#pragma endregion �p�[�e�B�N������
	}

	//�����p�[�e�B�N���X�V
	bomParticles.remove_if([](std::unique_ptr<SingleParticle>& newparticle)
		{
			return newparticle->frame == newparticle->num_frame;
		});
	for (std::unique_ptr<SingleParticle>& newparticle : bomParticles)
	{
		newparticle->updata();
	}

	//���p�[�e�B�N���X�V
	smokeParticles.remove_if([](std::unique_ptr<SingleParticle>& newparticle)
		{
			return newparticle->frame == newparticle->num_frame;
		});
	for (std::unique_ptr<SingleParticle>& newparticle : smokeParticles)
	{
		newparticle->updata();
	}

	//������������
	if (fallDownCount >= maxFallCount * 3)
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

void Boss::bossChase()
{
	if (enemyMovePattern != enemyPattern::chase)
	{
		return;
	}

	if (isAppear)
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

void Boss::bossShot()
{
	if (enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	if (isAppear)
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