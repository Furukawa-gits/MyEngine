#include"Enemy.h"
#include<random>

#pragma region ìGÇÃíe
enemyBullet::enemyBullet()
{
}
enemyBullet::~enemyBullet()
{
}
void enemyBullet::init(int index)
{

}

void enemyBullet::set(XMFLOAT3 start_pos, XMFLOAT3 Target)
{

}

void enemyBullet::update()
{

}

void enemyBullet::draw()
{

}
#pragma endregion

#pragma region ìGñ{ëÃ
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(testCube);
	delete(testObject);
}

void Enemy::init()
{
	Isarive = false;

	rockTarget.anchorpoint = { 0.5f,0.5f };
	rockTarget.size = { 70,70 };
	rockTarget.GenerateSprite("Rock_on.png");

	testCube = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	testObject = new Object3d_FBX;
	testObject->Initialize();
	testObject->SetModel(testCube);
	testObject->SetScale({ 1.0f,1.0f,1.0f });

	enemyCollision.radius = 2.0f;
}

void Enemy::set(XMFLOAT3 pos, enemyPattern pattern)
{
	position = pos;
	startPosition = pos;
	testObject->SetPosition(pos);
	Isarive = true;
	isTargetSet = false;
	chaseCount = 0;
	waitCount = 0;
	isChase = false;
	isWait = true;
	enemyMovePattern = pattern;
}

void Enemy::reSet()
{
	testObject->SetPosition(startPosition);
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
	//í«ê’
	if (isChase)
	{
		//í«îˆÉJÉEÉìÉgâ¡éZ
		chaseCount++;
		enemySpeed = 0.4f;
		if (chaseCount >= 20)
		{
			isChase = false;
			chaseCount = 0;
			isWait = true;
		}
	}

	//ë“ã@
	if (isWait)
	{
		//ë“ã@ÉJÉEÉìÉgâ¡éZ
		waitCount++;
		enemySpeed = 0.0f;
		if (waitCount >= 30)
		{
			isWait = false;
			waitCount = 0;
			isChase = true;
		}
	}

	position = testObject->getPosition();
	XMFLOAT3 dis = { pPos.x - position.x,pPos.y - position.y,pPos.z - position.z };

	float lengthDis = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	dis.x /= lengthDis;
	dis.y /= lengthDis;
	dis.z /= lengthDis;

	position.x += dis.x * enemySpeed;
	position.y += dis.y * enemySpeed;
	position.z += dis.z * enemySpeed;
}

void Enemy::update(XMFLOAT3 playerPos)
{
	if (!isDraw)
	{
		return;
	}

	//ìGÇ™ê∂ë∂
	ariveMove(playerPos);

	//ìGÇ™åÇíƒ
	deathMove();
	
	testObject->Update();

	return;
}

void Enemy::ariveMove(XMFLOAT3 playerPos)
{
	if (!Isarive)
	{
		return;
	}

	chase(playerPos);

	//HPÇ™0Ç…Ç»Ç¡ÇΩÇÁè¡ñ≈
	if (HP <= 0)
	{
		Isarive = false;
	}

	if (isTargetSet)
	{
		rockTarget.rotation += 1.5f;
		XMFLOAT2 screenPos = testObject->worldToScleen();
		rockTarget.position = { screenPos.x,screenPos.y,0 };
	}

	rockTarget.SpriteTransferVertexBuffer();
	rockTarget.SpriteUpdate();

	testObject->SetPosition(position);
	enemyCollision.center =
	{
		testObject->getPosition().x,
		testObject->getPosition().y,
		testObject->getPosition().z,1.0f
	};
}

void Enemy::deathMove()
{
	if (Isarive)
	{
		return;
	}

	position.y -= 0.2f;
	rot.x += 4;
	rot.y += 4;
	rot.z += 4;
	testObject->setRotMatrix(rot.x, rot.y, rot.z);

	if (position.y <= 0)
	{
		isDraw = false;
	}
}

void Enemy::isHitTarget(XMFLOAT2 targetpos, bool istarget)
{
	if (!Isarive)
	{
		return;
	}

	XMFLOAT2 screenPos = testObject->worldToScleen();

	float dis = sqrtf(powf(targetpos.x - screenPos.x, 2) + powf(targetpos.y - screenPos.y, 2));

	if (dis < 20 && istarget && !isTargetSet)
	{
		isTargetSet = true;
	}

	rockTarget.position = { screenPos.x,screenPos.y,0 };
}

void Enemy::isHitShot(XMFLOAT2 targetpos)
{
	if (!Isarive)
	{
		return;
	}

	XMFLOAT2 screenPos = testObject->worldToScleen();

	float dis = sqrtf(powf(targetpos.x - screenPos.x, 2) + powf(targetpos.y - screenPos.y, 2));

	if (dis < 30)
	{
		HP--;

		if (HP <= 0)
		{
			Isarive = false;
		}
	}
}

void Enemy::draw3D(directX* directx)
{
	if (!isDraw)
	{
		return;
	}

	testObject->SetPipelineSimple(directx->cmdList.Get());
	testObject->Draw(directx->cmdList.Get());
}

void Enemy::draw2D(directX* directx)
{
	if (Isarive && isTargetSet)
	{
		rockTarget.DrawSprite(directx->cmdList.Get());
	}
}
#pragma endregion