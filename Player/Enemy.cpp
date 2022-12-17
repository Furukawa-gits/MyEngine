#include"Enemy.h"
#include<random>

std::unique_ptr<Model> Enemy::enemyModelS = std::make_unique<Model>();
const float Enemy::forPlayer = 200.0f;

#pragma region �G�{��
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(enemyObject);
}

void Enemy::staticInit()
{
	enemyModelS.reset(FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01"));

	SingleParticle::loadTexInMap("bomb.png");
	SingleParticle::loadTexInMap("smoke.png");

	enemyBullet::staticInit();
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
	Isarive = true;
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

void Enemy::chase(XMFLOAT3 pPos)
{
	if (enemyMovePattern != enemyPattern::chase)
	{
		return;
	}

	//�ǐ�
	if (isChase)
	{
		//�ǔ��J�E���g���Z
		chaseCount++;
		enemySpeed = 0.25f;
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
	XMFLOAT3 dis = { pPos.x - position.x,pPos.y - position.y,pPos.z - position.z };

	float lengthDis = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	dis.x /= lengthDis;
	dis.y /= lengthDis;
	dis.z /= lengthDis;

	position.x += dis.x * enemySpeed;
	position.y += dis.y * enemySpeed;
	position.z += dis.z * enemySpeed;
}

void Enemy::shot(XMFLOAT3 pPos)
{
	if (enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

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

void Enemy::update(XMFLOAT3 playerPos)
{
	if (!isDraw)
	{
		return;
	}

	enemyObject->Update();

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

	//�G������
	ariveMove(playerPos);

	//�G������
	deathMove();

	return;
}

void Enemy::ariveMove(XMFLOAT3 playerPos)
{
	if (!Isarive)
	{
		return;
	}

	chase(playerPos);

	shot(playerPos);

	//HP��0�ɂȂ��������
	if (HP <= 0)
	{
		Isarive = false;
		fallDownCount = 0;

		for (int i = 0; i < PublicParticlenum; i++)
		{
#pragma region �����p�[�e�B�N������
			std::unique_ptr<SingleParticle> newparticle = std::make_unique<SingleParticle>();
			newparticle->generate();
			XMFLOAT3 vec =
			{
				(float)(rand() % 10 - 5) * 0.05f,
				(float)(rand() % 10 - 5) * 0.05f,
				(float)(rand() % 10 - 5) * 0.05f
			};
			newparticle->set(maxFallCount - 20, position, vec, { 0,0,0 }, 0.2, 5.0);

			bomParticles.push_back(std::move(newparticle));
#pragma endregion �����p�[�e�B�N������
		}
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

void Enemy::deathMove()
{
	if (Isarive)
	{
		return;
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

	fallDownCount++;

	position.y -= 0.2f;
	rot.x += deathRotSpeed;
	rot.y += deathRotSpeed;
	rot.z += deathRotSpeed;
	enemyObject->setRotMatrix(rot.x, rot.y, rot.z);

#pragma region �����p�[�e�B�N��
	if (fallDownCount % 15 == 0)
	{
		std::unique_ptr<SingleParticle> newparticle = std::make_unique<SingleParticle>();
		newparticle->generate();
		newparticle->set((maxFallCount - fallDownCount) + 10, position, { 0,0,0 }, { 0,0,0 }, 3.5, 0.5);

		smokeParticles.push_back(std::move(newparticle));
	}
#pragma endregion �����p�[�e�B�N��

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

	enemyObject->SetPipelineSimple(directx->cmdList.Get());
	enemyObject->Draw(directx->cmdList.Get());

	if (enemyMovePattern == enemyPattern::shot)
	{
		bullet->draw(directx);
	}

	//�����p�[�e�B�N���`��
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
	if (isFar)
	{
		return;
	}

	if (Isarive && isTargetSet)
	{
		rockTarget->DrawSprite(directx->cmdList.Get());
	}
}
#pragma endregion

#pragma region �G�̒e
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