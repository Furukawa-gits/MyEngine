#include"Bullets.h"
#include"../3D/Collision.h"
#include"../3D/3Dobject.h"

#pragma region 通常弾

bullet::bullet()
{
}

bullet::~bullet()
{
}

void bullet::staticInit()
{
	SingleParticle::loadTexInMap("effect1.png");
}

void bullet::staticDestroy()
{
	
}

void bullet::init()
{
	//親パーティクル生成
	motherParticle = std::make_unique<SingleParticle>();
	motherParticle->generate();
	motherParticle->set(0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f, false, true);
	motherParticle->color = { 0,1,0,1 };

	bulletCollision.radius = 4.0f;
}

void bullet::set(XMFLOAT3 start_pos, XMFLOAT3 Target)
{
	position = start_pos;

	XMFLOAT3 dis = {
		Target.x - start_pos.x,
		Target.y - start_pos.y,
		Target.z - start_pos.z,
	};

	float length = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	bulletVec = { (dis.x / length) * bulletSpeed,(dis.y / length) * bulletSpeed ,(dis.z / length) * bulletSpeed };

	isAlive = true;
}

void bullet::update()
{
	if (!isAlive)
	{
		return;
	}

	addBulletVec();

	count++;

	//一定フレームごとにパーティクルを生成
	if (count % 5 == 0)
	{
		SingleParticle newParticle;
		newParticle.generate();
		newParticle.set(30, position, { 0,0,0 }, { 0,0,0 }, 2.0f, 0.0f);
		newParticle.color = { 0,1,0,1 };
		newParticle.isAddBlend = true;
		particleManagerOnTime::addParticle(newParticle, "effect1.png");
	}

	//弾の寿命が来たら消滅
	if (count >= maxBulletCount)
	{
		count = 0;
		motherParticle->setIsActive(false);
		isAlive = false;
	}

	//本体パーティクル更新
	motherParticle->setPosition(position);
	motherParticle->updata();

	bulletCollision.center = XMLoadFloat3(&position);
}

void bullet::draw(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	motherParticle->setPiplineAddBlend();
	motherParticle->drawSpecifyTex("effect1.png");
}
#pragma endregion

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
	XMFLOAT3 to_enemy = {
		enemyPointer->position.x - position.x,
		enemyPointer->position.y - position.y,
		enemyPointer->position.z - position.z
	};

	//ベクトルを正規化
	XMFLOAT3 bullet_vec_nml = normalized(bulletVec);

	//現在の進行方向との内積
	float dot_ene_bullet =
		to_enemy.x * bulletVec.x +
		to_enemy.y * bulletVec.y +
		to_enemy.z * bulletVec.z;

	//ベクトルの外積を計算
	XMFLOAT3 closs_bullet_vec = {
		bullet_vec_nml.x * dot_ene_bullet,
		bullet_vec_nml.y * dot_ene_bullet,
		bullet_vec_nml.z * dot_ene_bullet
	};

	//ミサイルの進行ベクトルをターゲットの方に曲げるベクトルを計算
	XMFLOAT3 centripetalAccel = {
		position.x - closs_bullet_vec.x,
		position.y - closs_bullet_vec.y,
		position.z - closs_bullet_vec.z
	};

	XMFLOAT3 centri_to_enemy = {
		enemyPointer->position.x - centripetalAccel.x,
		enemyPointer->position.y - centripetalAccel.y,
		enemyPointer->position.z - centripetalAccel.z
	};

	float centri_to_enemyMagnitude = returnScaler(centri_to_enemy);
	if (centri_to_enemyMagnitude > 1.0f)
	{
		centri_to_enemy = normalized(centri_to_enemy);
	}

	//曲げる力に補正を入れる
	XMFLOAT3 Force = {
		centri_to_enemy.x * 2.3f,
		centri_to_enemy.y * 2.3f,
		centri_to_enemy.z * 2.3f
	};

	Force.x += bullet_vec_nml.x * 0.7f;
	Force.y += bullet_vec_nml.y * 0.7f;
	Force.z += bullet_vec_nml.z * 0.7f;

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
		newParticle.set(20, position, { 0,0,0 }, { 0,0,0 }, 2.0f, 0.0f);
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