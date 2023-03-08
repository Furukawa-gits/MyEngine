#include"Bullets.h"
#include"../3D/Collision.h"
#include"../3D/3Dobject.h"

#pragma region 通常弾
std::unique_ptr<Model> bullet::bulletModelS = std::make_unique<Model>();

bullet::bullet()
{
}

bullet::~bullet()
{
	delete(bulletObject);
}

void bullet::staticInit()
{
	bulletModelS.reset(FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01"));
}

void bullet::staticDestroy()
{
	
}

void bullet::init()
{
	bulletObject = new Object3d_FBX;
	bulletObject->Initialize();
	bulletObject->SetModel(bulletModelS.get());
	bulletObject->SetScale({ 0.5f,0.5f,0.5f });

	bulletCollision.radius = 4.0f;
}

void bullet::set(XMFLOAT3 start_pos, XMFLOAT3 Target)
{
	bulletObject->SetPosition({ start_pos.x,start_pos.y,start_pos.z });

	XMFLOAT3 dis = {
		Target.x - start_pos.x,
		Target.y - start_pos.y,
		Target.z - start_pos.z,
	};

	float length = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	bullet_vec = { (dis.x / length) * bulletSpeed,(dis.y / length) * bulletSpeed ,(dis.z / length) * bulletSpeed };

	isArive = true;
}

void bullet::update()
{
	if (!isArive)
	{
		return;
	}

	bulletObject->addMoveFront(bullet_vec);

	count++;

	if (count >= 100)
	{
		count = 0;
		isArive = false;
	}

	bulletObject->Update();
	bulletCollision.center = {
		bulletObject->getPosition().x,
		bulletObject->getPosition().y,
		bulletObject->getPosition().z,
		1.0f };
}

void bullet::draw(directX* directx)
{
	if (!isArive)
	{
		return;
	}

	bulletObject->Draw(directx->cmdList.Get());
}
#pragma endregion

#pragma region ミサイル
std::unique_ptr<Model> Missile::MissileModelS = std::make_unique<Model>();

Missile::Missile()
{
}

Missile::~Missile()
{
	delete(bulletObject);
}

void Missile::staticInit()
{
	MissileModelS.reset(FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01"));
}

void Missile::staticDestroy()
{
}

void Missile::init()
{
	bulletObject = new Object3d_FBX;
	bulletObject->Initialize();
	bulletObject->SetModel(MissileModelS.get());
	bulletObject->SetScale({ 0.5f,0.5f,0.5f });

	bulletObject->setColor({ 1,1,0,1 });

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
	bulletObject->SetPosition(start_pos);

	isArive = true;
}

void Missile::update()
{
	//セットされていないミサイルは更新処理を行わない
	if (!isArive)
	{
		return;
	}

	//ミサイルが当たる前にターゲットがいなくなれば次フレームから更新しない
	if (enemyPointer->Isarive == false)
	{
		isArive = false;
	}

	//ターゲットへのベクトル
	XMFLOAT3 to_enemy = {
		enemyPointer->position.x - bulletObject->getPosition().x,
		enemyPointer->position.y - bulletObject->getPosition().y,
		enemyPointer->position.z - bulletObject->getPosition().z
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
		bulletObject->getPosition().x - closs_bullet_vec.x,
		bulletObject->getPosition().y - closs_bullet_vec.y,
		bulletObject->getPosition().z - closs_bullet_vec.z
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

	bulletObject->addMoveFront(bulletVec);

	bulletObject->Update();

	missileCollision.center = {
	bulletObject->getPosition().x,
	bulletObject->getPosition().y,
	bulletObject->getPosition().z,
	1.0f };

}

void Missile::draw(directX* directx)
{
	if (!isArive)
	{
		return;
	}

	bulletObject->SetPipelineSimple(directx->cmdList.Get());
	bulletObject->Draw(directx->cmdList.Get());
}
#pragma endregion