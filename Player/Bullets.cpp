#include"Bullets.h"
#include"../3D/Collision.h"
#include"../3D/3Dobject.h"

#pragma region ’Êí’e
bullet::bullet()
{
}

bullet::~bullet()
{
	delete(bulletObject);
	delete(bulletModel);
}

void bullet::init()
{
	bulletModel = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	bulletObject = new Object3d_FBX;
	bulletObject->Initialize();
	bulletObject->SetModel(bulletModel);
	bulletObject->SetScale({ 0.5f,0.5f,0.5f });

	bulletCollision.radius = 2.0f;
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

void bullet::checkHitEnemy(Enemy* enemy)
{
	if (!enemy->Isarive || !isArive)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(this->bulletCollision, enemy->enemyCollision))
	{
		count = 0;
		isArive = false;

		enemy->HP--;
	}
}

void bullet::checkHitEnemyBullet(Enemy* enemy)
{
	if (enemy->enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	if (!enemy->bullet.isBulletArive())
	{
		return;
	}

	if (!isArive)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(this->bulletCollision, enemy->bullet.bulletCollision))
	{
		count = 0;
		isArive = false;

		enemy->bullet.isArive = false;
		enemy->bullet.ariveTime = 0;
	}
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

#pragma region ƒ~ƒTƒCƒ‹
Missile::Missile()
{
}

Missile::~Missile()
{
	delete(bulletObject);
	delete(bulletModel);
}

void Missile::init()
{
	bulletModel = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	bulletObject = new Object3d_FBX;
	bulletObject->Initialize();
	bulletObject->SetModel(bulletModel);
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

void Missile::checkhit()
{
	if (enemyPointer == nullptr)
	{
		return;
	}

	if (enemyPointer->Isarive == true && isArive == true)
	{
		if (Collision::CheckSphere2Sphere(this->missileCollision, enemyPointer->enemyCollision))
		{
			isArive = false;
			isTargetSet = false;

			enemyPointer->Isarive = false;

			enemyPointer = nullptr;
		}
	}
}

void Missile::update()
{
	if (!isArive)
	{
		return;
	}

	if (enemyPointer->Isarive == false)
	{
		isArive = false;
	}

	XMFLOAT3 to_enemy = {
		enemyPointer->position.x - bulletObject->getPosition().x,
		enemyPointer->position.y - bulletObject->getPosition().y,
		enemyPointer->position.z - bulletObject->getPosition().z
	};

	XMFLOAT3 bullet_vec_nml = normalized(bulletVec);

	float dot_ene_bullet =
		to_enemy.x * bulletVec.x +
		to_enemy.y * bulletVec.y +
		to_enemy.z * bulletVec.z;

	XMFLOAT3 closs_bullet_vec = {
		bullet_vec_nml.x * dot_ene_bullet,
		bullet_vec_nml.y * dot_ene_bullet,
		bullet_vec_nml.z * dot_ene_bullet
	};

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

	XMFLOAT3 Force = {
		centri_to_enemy.x * 2.0f,
		centri_to_enemy.y * 2.0f,
		centri_to_enemy.z * 2.0f
	};

	Force.x += bullet_vec_nml.x * 0.7f;
	Force.y += bullet_vec_nml.y * 0.7f;
	Force.z += bullet_vec_nml.z * 0.7f;

	Force.x -= bulletVec.x * 1.2f;
	Force.y -= bulletVec.y * 1.2f;
	Force.z -= bulletVec.z * 1.2f;

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

	checkhit();

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