#include"Bullets.h"
#include"../3D/Collision.h"

#pragma region ’Êí’e
void bullet::init(int index)
{
	bullet_view = object3D_obj::objectcommon->ReturnModelViewes("player_bullet.obj", 1);

	object.Init3d(index);

	object.scale = { 2.0f,2.0f ,2.0f };

	bullet_collision.radius = 3.0f;
}

void bullet::set(XMFLOAT3 start_pos, XMFLOAT3 Target)
{
	object.position = { start_pos.x,start_pos.y,start_pos.z };

	XMVECTOR sc_pos = object.WorldToScreenPos(XMLoadFloat3(&start_pos));

	float xdis = Target.x - sc_pos.m128_f32[0];
	float ydis = Target.y - sc_pos.m128_f32[1];

	float xy_vec = sqrtf(xdis * xdis + ydis * ydis);

	XMFLOAT2 normalize;

	normalize.x = xdis / xy_vec;
	normalize.y = ydis / xy_vec;

	if (xy_vec > 150)
	{
		bullet_vec = { normalize.x * 2.0f,-(normalize.y * 2.0f),2.0f };
	}
	else
	{
		bullet_vec = { normalize.x * 0.5f,-(normalize.y * 0.5f),2.0f };
	}

	XMFLOAT3 set_vec = { normalize.x * 2.0f,-(normalize.y * 2.0f),(1 / xy_vec) * 900.0f };

	XMFLOAT3 set_vec_nml = normalized(set_vec);

	bullet_vec = { set_vec_nml.x * 3.0f,(set_vec_nml.y * 3.0f),set_vec_nml.z * 3.0f };

	Isarive = true;
}

//“G‚Æ‚Ì“–‚½‚è”»’è
void bullet::checkhit(Enemy* enemy)
{
	if (enemy->Isarive == true && Isarive == true)
	{
		if (Collision::CheckSphere2Sphere(this->bullet_collision, enemy->enemy_collision))
		{
			count = 0;
			Isarive = false;

			enemy->IsRockon_draw = false;
			enemy->Isshot = false;
			enemy->Isarive = false;
		}
	}
}

void bullet::update()
{
	if (Isarive == true)
	{
		object.rotation.x += 3.0f;
		object.rotation.y += 3.0f;
		object.rotation.z += 3.0f;

		object.position.x += bullet_vec.x;
		object.position.y += bullet_vec.y;
		object.position.z += bullet_vec.z;

		count++;

		if (count >= 100)
		{
			count = 0;
			Isarive = false;
		}
	}

	object.Update3d(bullet_view, { 1.0f,1.0f ,0.0f,1.0f });
	bullet_collision.center = {
		object.position.x,
		object.position.y,
		object.position.z,
		1.0f };
}

void bullet::draw()
{
	if (Isarive == true)
	{
		object.DrawModel_OnMaterial(bullet_view);
	}
}
#pragma endregion

#pragma region ƒ~ƒTƒCƒ‹
void Missile::init(int index)
{
	misslie_view = object3D_obj::objectcommon->ReturnModelViewes("player_bullet.obj", 2);

	object.Init3d(index);

	missile_collision.radius = 3.0f;
}

void Missile::setPenemy(Enemy* enemy)
{
	P_enemy = enemy;
	IsTarget_set = true;
}

void Missile::start(XMFLOAT3 start_pos)
{
	bullet_vec = bullet_vec_index[rand() % 8];
	object.position = start_pos;

	if (P_enemy != nullptr && IsTarget_set == true)
	{
		Isarive = true;
	}
}

void Missile::checkhit()
{
	if (P_enemy == nullptr)
	{
		return;
	}

	if (P_enemy->Isarive == true && Isarive == true)
	{
		if (Collision::CheckSphere2Sphere(this->missile_collision, P_enemy->enemy_collision))
		{
			Isarive = false;
			IsTarget_set = false;

			P_enemy->IsRockon_draw = false;
			P_enemy->Isshot = false;
			P_enemy->Isarive = false;
		}
	}
}

void Missile::update()
{
	if (Isarive == true)
	{
		if (Isarive == true && P_enemy->Isarive == false)
		{
			Isarive = false;
		}

		XMFLOAT3 to_enemy = {
			P_enemy->enemy.position.x - object.position.x,
			P_enemy->enemy.position.y - object.position.y,
			P_enemy->enemy.position.z - object.position.z
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
			object.position.x - closs_bullet_vec.x,
			object.position.y - closs_bullet_vec.y,
			object.position.z - closs_bullet_vec.z
		};

		XMFLOAT3 centri_to_enemy = {
			P_enemy->enemy.position.x - centripetalAccel.x,
			P_enemy->enemy.position.y - centripetalAccel.y,
			P_enemy->enemy.position.z - centripetalAccel.z
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

		object.rotation.x += 50.0f;
		object.rotation.y += 50.0f;
		object.rotation.z += 50.0f;

		object.position.x += bullet_vec.x;
		object.position.y += bullet_vec.y;
		object.position.z += bullet_vec.z;
		object.Update3d(misslie_view, { 0.32f,1.0f,0.562f,1.0f });

		missile_collision.center = {
		object.position.x,
		object.position.y,
		object.position.z,
		1.0f };
	}
}

void Missile::draw()
{
	if (Isarive == true)
	{
		object.DrawModel_OnMaterial(misslie_view);
	}
}
#pragma endregion