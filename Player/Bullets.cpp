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
void bullet::init(int index)
{
	bulletModel = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	bulletObject = new Object3d_FBX;
	bulletObject->Initialize();
	bulletObject->SetModel(bulletModel);
	bulletObject->SetScale({ 0.5f,0.5f,0.5f });

	bullet_collision.radius = 2.0f;
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

	bullet_vec = { (dis.x / length) * 3.0f,(dis.y / length) * 3.0f ,(dis.z / length) * 3.0f };

	isArive = true;
}

//“G‚Æ‚Ì“–‚½‚è”»’è
void bullet::checkhit(Enemy* enemy)
{
	if (enemy->Isarive == true && isArive == true)
	{
		if (Collision::CheckSphere2Sphere(this->bullet_collision, enemy->enemyCollision))
		{
			count = 0;
			isArive = false;

			enemy->HP--;
		}
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
	bullet_collision.center = {
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
void Missile::init(int index)
{
	bulletModel = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	bulletObject = new Object3d_FBX;
	bulletObject->Initialize();
	bulletObject->SetModel(bulletModel);
	bulletObject->SetScale({ 0.5f,0.5f,0.5f });

	bulletObject->setColor({ 1,1,0,1 });

	missile_collision.radius = 3.0f;
}

void Missile::setPenemy(Enemy* enemy)
{
	P_enemy = enemy;
	IsTarget_set = true;
}

void Missile::start(XMFLOAT3 start_pos)
{
	if (!IsTarget_set || P_enemy == nullptr)
	{
		return;
	}

	bullet_vec = bullet_vec_index[rand() % 8];
	bulletObject->SetPosition(start_pos);

	Isarive = true;
}

void Missile::checkhit()
{
	if (P_enemy == nullptr)
	{
		return;
	}

	if (P_enemy->Isarive == true && Isarive == true)
	{
		if (Collision::CheckSphere2Sphere(this->missile_collision, P_enemy->enemyCollision))
		{
			Isarive = false;
			IsTarget_set = false;

			P_enemy->Isarive = false;

			P_enemy = nullptr;
		}
	}
}

void Missile::update()
{
	if (!Isarive)
	{
		return;
	}

	if (P_enemy->Isarive == false)
	{
		Isarive = false;
	}

	XMFLOAT3 to_enemy = {
		P_enemy->position.x - bulletObject->getPosition().x,
		P_enemy->position.y - bulletObject->getPosition().y,
		P_enemy->position.z - bulletObject->getPosition().z
	};

	XMFLOAT3 bullet_vec_nml = normalized(bullet_vec);

	float dot_ene_bullet =
		to_enemy.x * bullet_vec.x +
		to_enemy.y * bullet_vec.y +
		to_enemy.z * bullet_vec.z;

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
		P_enemy->position.x - centripetalAccel.x,
		P_enemy->position.y - centripetalAccel.y,
		P_enemy->position.z - centripetalAccel.z
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

	Force.x -= bullet_vec.x * 1.2f;
	Force.y -= bullet_vec.y * 1.2f;
	Force.z -= bullet_vec.z * 1.2f;

	bullet_vec.x += Force.x;
	bullet_vec.y += Force.y;
	bullet_vec.z += Force.z;

	bulletObject->addMoveFront(bullet_vec);

	bulletObject->Update();

	missile_collision.center = {
	bulletObject->getPosition().x,
	bulletObject->getPosition().y,
	bulletObject->getPosition().z,
	1.0f };

	checkhit();

}

void Missile::draw(directX* directx)
{
	if (!Isarive)
	{
		return;
	}

	bulletObject->SetPipelineSimple(directx->cmdList.Get());
	bulletObject->Draw(directx->cmdList.Get());
}
#pragma endregion