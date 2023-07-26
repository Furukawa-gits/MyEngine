#include"HomingMissile.h"
#include"../3D/Collision.h"
#include"../3D/3Dobject.h"
#include <random>

Missile::Missile()
{
}

Missile::~Missile()
{
}

void Missile::init(XMFLOAT4 motherColor, XMFLOAT4 childColor)
{
	//弾の本体パーティクル初期化
	motherParticle = std::make_unique<SingleParticle>();
	motherParticle->generate();
	motherParticle->set(0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f, false, true);
	motherParticle->color = motherColor;

	this->childColor = childColor;

	bulletCollision.radius = 3.0f;
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

#pragma region 制御点を設定
	//乱数のシード値を決定
	std::random_device seed;
	std::mt19937 rnd(seed());
	std::uint32_t xResult = rnd();
	std::uint32_t yResult = rnd();
	std::uint32_t zResult = rnd();

	//初期位置にプレイヤー位置・終端位置に敵の位置を設定
	RKDVector3 start(start_pos.x, start_pos.y, start_pos.z);
	RKDVector3 end(
		enemyPointer->position.x, 
		enemyPointer->position.y,
		enemyPointer->position.z);

	//2個目の制御点のオフセット(ランダム)
	RKDVector3 randPointOne =
	{
		(float)(xResult % 30) - 15,
		(float)(yResult % 30) - 15,
		(float)(zResult % 30) - 15
	};

	//3個目の制御点のオフセット(ランダム)
	xResult = rnd();
	yResult = rnd();
	zResult = rnd();
	RKDVector3 randPointTwo =
	{
		(float)(xResult % 30) - 15,
		(float)(yResult % 30) - 15,
		(float)(zResult % 30) - 15
	};
#pragma endregion 制御点を設定

	//制御点をまとめる
	std::array<RKDVector3, 6> setPoints;
	setPoints[0] = start;
	setPoints[1] = start;
	setPoints[2] = start + randPointOne;
	setPoints[3] = end + randPointTwo;
	setPoints[4] = end;
	setPoints[5] = end;

	//スプライン曲線にセット
	missileCurve.setSpline(setPoints.data(), 6, toEnemyMaxFrame);
	missileCurve.play();

	//スプライン曲線状の通過点を保存
	for (int i = 0; i < toEnemyMaxFrame; i++)
	{
		missilePositionts.push_back(missileCurve.updata());
	}
}

void Missile::updata()
{
	if (!isAlive)
	{
		return;
	}

	//敵に到達する前に寿命が尽きた時
	if (aliveFrame >= toEnemyMaxFrame)
	{
		enemyPointer->isTargetSet = false;
		enemyPointer->isSetMissile = false;
		isAlive = false;
	}

	if (enemyPointer->isAlive == false)
	{
		enemyPointer = nullptr;
		isAlive = false;
	}

	//フレームに応じた通過点をセット
	position = missilePositionts[aliveFrame];
	aliveFrame++;

	//パーティクル更新
	particleUpdata();

	bulletCollision.center = XMLoadFloat3(&position);
}

void Missile::particleUpdata()
{
	//パーティクル生成
	SingleParticle newParticle;
	newParticle.generate();
	newParticle.set(30, position, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f);
	newParticle.color = { 1,1,0,1 };
	newParticle.isAddBlend = true;
	particleManagerOnTime::addParticle(newParticle, "effect1.png");

	//パーティクル更新
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