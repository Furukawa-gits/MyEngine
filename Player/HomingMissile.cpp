#include"HomingMissile.h"
#include"../3D/Collision.h"
#include"../3D/3Dobject.h"
#include <random>

#pragma region ミサイル

Missile::Missile()
{
}

Missile::~Missile()
{
}

void Missile::staticInit()
{

}

void Missile::staticDestroy()
{
}

void Missile::init()
{
	//親パーティクル生成
	motherParticle = std::make_unique<SingleParticle>();
	motherParticle->generate();
	motherParticle->set(0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f, false, true);
	motherParticle->color = { 1,1,0,1 };

	missileCollision.radius = 3.0f;
}

void Missile::setPenemy(Enemy* enemy)
{
	enemyPointer = enemy;
	isTargetSet = true;
}

void Missile::start(XMFLOAT3 start_pos)
{
	if (!isTargetSet || enemyPointer == nullptr)
	{
		return;
	}

	bulletVec = bulletVecIndex[rand() % 8];
	position = start_pos;

	isAlive = true;

	std::random_device seed;
	std::mt19937 rnd(seed());

	std::uint32_t xResult = rnd();
	std::uint32_t yResult = rnd();
	std::uint32_t zResult = rnd();

	RKDVector3 start(start_pos.x, start_pos.y, start_pos.z);
	RKDVector3 end(enemyPointer->position.x, enemyPointer->position.y, enemyPointer->position.z);
	RKDVector3 randPoint = 
	{ 
		(float)(xResult % 30) - 15,
		(float)(yResult % 30) - 15,
		(float)(zResult % 30) - 15 
	};

	std::array<RKDVector3, 6> setPoints;
	setPoints[0] = start;
	setPoints[1] = start;
	setPoints[2] = start + randPoint;
	setPoints[3] = end + randPoint;
	setPoints[4] = end;
	setPoints[5] = end;

	missileCurve.setSpline(setPoints.data(), 6, toEnemyMaxFrame);
	missileCurve.play();

	for (int i = 0; i < toEnemyMaxFrame; i++)
	{
		missilePositionts.push_back(missileCurve.updata());
	}
}

void Missile::update()
{
	//セットされていないミサイルは更新処理を行わない
	if (!isAlive)
	{
		return;
	}

	if (aliveFrame >= toEnemyMaxFrame)
	{
		isAlive = false;
	}

	//ミサイルが当たる前にターゲットがいなくなれば次フレームから更新しない
	if (enemyPointer->isAlive == false)
	{
		isAlive = false;
	}

	position = missilePositionts[aliveFrame];

	aliveFrame++;

	//パーティクル更新
	particleUpdata();

	missileCollision.center = XMLoadFloat3(&position);
}

void Missile::particleUpdata()
{
	//パーティクル用のカウント
	particleCount++;

	//一定フレームごとにパーティクルを生成
	if (particleCount % 2 == 0)
	{
		SingleParticle newParticle;
		newParticle.generate();
		newParticle.set(30, position, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f);
		newParticle.color = { 1,1,0,1 };
		newParticle.isAddBlend = true;
		particleManagerOnTime::addParticle(newParticle, "effect1.png");
	}

	//本体パーティクル更新
	motherParticle->setPosition(position);
	motherParticle->updata();
}

void Missile::draw(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	motherParticle->setPiplineAddBlend();
	motherParticle->drawSpecifyTex("effect1.png");
}
#pragma endregion