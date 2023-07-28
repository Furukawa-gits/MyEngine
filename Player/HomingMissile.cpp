#include"HomingMissile.h"

Missile::Missile()
{
	motherParticle = std::make_unique<SingleParticle>();
}

Missile::~Missile()
{
}

void Missile::init(XMFLOAT4 motherColor, XMFLOAT4 childColor)
{
	
	motherParticle->generate();
	motherParticle->set(0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f, false, true);
	motherParticle->color = motherColor;

	this->childColor = childColor;

	bulletCollision.radius = 3.0f;

	particleUpdata();
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

void Missile::draw(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	motherParticle->setPiplineAddBlend();
	motherParticle->drawSpecifyTex("effect1.png");
}

void Missile::setPenemy(Enemy* enemy)
{
	enemyPointer = enemy;
	isTargetSet = true;
}

void Missile::start(XMFLOAT3 start_pos)
{
	if (!this->isTargetSet || this->enemyPointer == nullptr)
	{
		return;
	}

	this->position = start_pos;

	this->isAlive = true;

	std::random_device seedNum;
	std::mt19937 rnd(seedNum());

	std::uint32_t xResult = rnd();
	std::uint32_t yResult = rnd();
	std::uint32_t zResult = rnd();

	RKDVector3 startPos(start_pos.x, start_pos.y, start_pos.z);
	RKDVector3 endPos(
		this->enemyPointer->position.x, 
		this->enemyPointer->position.y,
		this->enemyPointer->position.z);

	//2個目の制御点のオフセット(ランダム)
	RKDVector3 randPointOne(
		(float)(xResult % 30) - 15,
		(float)(yResult % 30) - 15,
		(float)(zResult % 30) - 15
	);

	//3個目の制御点のオフセット(ランダム)
	xResult = rnd();
	yResult = rnd();
	zResult = rnd();

	RKDVector3 randPointTwo(
		(float)(xResult % 30) - 15,
		(float)(yResult % 30) - 15,
		(float)(zResult % 30) - 15
	);

	//制御点をまとめる
	std::array<RKDVector3, 6> setPoints = {};
	setPoints[0] = startPos;
	setPoints[1] = startPos;
	setPoints[2] = startPos + randPointOne;
	setPoints[3] = endPos + randPointTwo;
	setPoints[4] = endPos;
	setPoints[5] = endPos;

	//スプライン曲線にセット
	this->missileCurve.setSpline(setPoints.data(), 6, this->toEnemyMaxFrame);
	this->missileCurve.play();

	//スプライン曲線状の通過点を保存
	for (int i = 0; i < toEnemyMaxFrame; i++)
	{
		this->missilePositionts.push_back(this->missileCurve.updata());
	}
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