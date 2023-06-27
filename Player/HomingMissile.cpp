#include"HomingMissile.h"
#include"../3D/Collision.h"
#include"../3D/3Dobject.h"


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
}

void Missile::update()
{
	//セットされていないミサイルは更新処理を行わない
	if (!isAlive)
	{
		return;
	}

	//ミサイルが当たる前にターゲットがいなくなれば次フレームから更新しない
	if (enemyPointer->isAlive == false)
	{
		isAlive = false;
	}

	//ターゲットへのベクトル
	XMFLOAT3 toEnemy = {
		enemyPointer->position.x - position.x,
		enemyPointer->position.y - position.y,
		enemyPointer->position.z - position.z
	};

	//ベクトルを正規化
	XMFLOAT3 missileVecNml = normalized(bulletVec);

	//現在の進行方向との内積
	float dotEneBullet =
		toEnemy.x * bulletVec.x +
		toEnemy.y * bulletVec.y +
		toEnemy.z * bulletVec.z;

	//ベクトルの外積を計算
	XMFLOAT3 clossBulletVec = {
		missileVecNml.x * dotEneBullet,
		missileVecNml.y * dotEneBullet,
		missileVecNml.z * dotEneBullet
	};

	//ミサイルの進行ベクトルをターゲットの方に曲げるベクトルを計算
	XMFLOAT3 centripetalAccel = {
		position.x - clossBulletVec.x,
		position.y - clossBulletVec.y,
		position.z - clossBulletVec.z
	};

	XMFLOAT3 centriToEnemy = {
		enemyPointer->position.x - centripetalAccel.x,
		enemyPointer->position.y - centripetalAccel.y,
		enemyPointer->position.z - centripetalAccel.z
	};

	float centriToEnemyMagnitude = returnScaler(centriToEnemy);
	if (centriToEnemyMagnitude > 1.0f)
	{
		centriToEnemy = normalized(centriToEnemy);
	}

	//曲げる力に補正を入れる
	XMFLOAT3 Force = {
		centriToEnemy.x * 2.3f,
		centriToEnemy.y * 2.3f,
		centriToEnemy.z * 2.3f
	};

	Force.x += missileVecNml.x * 0.7f;
	Force.y += missileVecNml.y * 0.7f;
	Force.z += missileVecNml.z * 0.7f;

	Force.x -= bulletVec.x * 1.2f;
	Force.y -= bulletVec.y * 1.2f;
	Force.z -= bulletVec.z * 1.2f;

	//ミサイルの進行方向を曲げる
	bulletVec.x += Force.x;
	bulletVec.y += Force.y;
	bulletVec.z += Force.z;

	bulletVec.x *= 1.5f;
	bulletVec.y *= 1.5f;
	bulletVec.z *= 1.5f;

	addBulletVec();

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