#include"Enemy.h"

#pragma region “G‚Ì’e
void enemy_bullet::init(int index)
{
	bullet_view = object3D::objectcommon->ReturnModelViewes("player_bullet.obj", 4);

	object.Init3d(index);

	object.scale = { 2.0f,2.0f ,2.0f };

	bullet_collision.radius = 3.0f;
}

void enemy_bullet::set(XMFLOAT3 start_pos, XMFLOAT3 Target)
{
	object.position = { start_pos.x,start_pos.y,start_pos.z };

	float xdis = Target.x - start_pos.x;
	float ydis = Target.y - start_pos.y;
	float zdis = Target.z - start_pos.y;

	XMFLOAT3 to_player = {
		Target.x - start_pos.x,
		Target.y - start_pos.y,
		Target.z - start_pos.z
	};

	XMFLOAT3 normalize = normalized(to_player);

	bullet_vec = { normalize.x * 2.5f,normalize.y * 2.5f,normalize.z * 2.5f };

	count = 0;
	Isarive = true;
}

void enemy_bullet::update()
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

		if (object.position.z <= -10)
		{
			Isarive = false;
		}
	}

	object.Update3d(bullet_view, { 0.2f,0.5f,1.0f,1.0f });
	bullet_collision.center = {
		object.position.x,
		object.position.y,
		object.position.z,
		1.0f };
}

void enemy_bullet::draw()
{
	if (Isarive == true)
	{
		object.DrawModel_OnMaterial(bullet_view);
	}
}
#pragma endregion

#pragma region “G–{‘Ì
void Enemy::init(int enemy_index, int enemy_bullet_index, TexManager* tex)
{
	enemy_view = object3D::objectcommon->ReturnModelViewes("enemy.obj", 3);
	enemy_view2 = object3D::objectcommon->ReturnModelViewes("enemy2.obj", 5);
	enemy_view3 = object3D::objectcommon->ReturnModelViewes("enemy3.obj", 6);

	bullet.init(enemy_bullet_index);

	enemy.Init3d(enemy_index);
	enemy.scale = { 3.0f,3.0f,3.0f };

	Isarive = true;

	Rock_Target.GenerateSprite(object3D::directx->dev.Get(), 1280, 720, 3, tex, false, false, false, false);

	enemy_collision.radius = 4.0f;
}

void Enemy::setenemy(int move_Pat, XMFLOAT3 position)
{
	Isarive = true;
	IsRockon_draw = false;
	Istarget_set = false;
	Isshot = false;
	move_flag = 1;
	enemy_move_Pat = move_Pat;
	enemy.position = position;
}

void Enemy::update(SpriteCommon* commonsp, XMFLOAT3 Player_pos)
{
	XMFLOAT4 color{};

	if (Isarive == true)
	{
		if (enemy_move_Pat == 1)
		{
			if (enemy.position.z >= 80)
			{
				enemy.position.z -= 0.5f;
			}

			if (enemy.position.x + enemy_speed > 150)
			{
				move_flag = -1;
			}
			if (enemy.position.x - enemy_speed < -150)
			{
				move_flag = 1;
			}

			enemy.position.x += enemy_speed * move_flag;

			color = { 1.0f,1.0f,1.0f,1.0f };

			enemy.rotation.x -= 2.0f;
			enemy.rotation.y -= 2.0f;
			enemy.Update3d(enemy_view, color);
		}
		else if (enemy_move_Pat == 2)
		{
			if (enemy.position.z <= 150)
			{
				if (Isshot == false)
				{
					bullet.set(enemy.position, Player_pos);

					Isshot = true;
				}
				else if (Isshot == true)
				{
					bullet.update();

					if (bullet.Isarive == false)
					{
						Isshot = false;
					}
				}
			}
			else
			{
				enemy.position.z -= 0.8f;
			}

			color = { 0.3f,0.3f,1.0f,1.0f };

			enemy.rotation.x -= 2.0f;
			enemy.rotation.y -= 2.0f;
			enemy.Update3d(enemy_view2, color);
		}
		else if (enemy_move_Pat == 3)
		{
			if (enemy.position.z <= 150)
			{
				if (Isshot == false)
				{
					bullet.set(enemy.position, Player_pos);

					Isshot = true;
				}
				else if (Isshot == true)
				{
					bullet.update();

					if (bullet.Isarive == false)
					{
						Isshot = false;
					}
				}
			}
			else
			{
				enemy.position.z -= 0.8f;
			}

			color = { 1.0f,0.3f,0.3f,1.0f };

			enemy.rotation.x -= 2.0f;
			enemy.rotation.y -= 2.0f;
			enemy.Update3d(enemy_view3, color);
		}
	}

	if (IsRockon_draw == true)
	{
		Rock_Target.rotation += 2.0f;
	}
	else
	{
		Rock_Target.rotation = 0.0f;
	}


	Rock_Target.position.x = enemy_sc_pos.m128_f32[0];
	Rock_Target.position.y = enemy_sc_pos.m128_f32[1];
	Rock_Target.SpriteUpdate(*commonsp);

	enemy_sc_pos = enemy.WorldToScreenPos(XMLoadFloat3(&enemy.position));

	enemy_collision.center = {
		enemy.position.x,
		enemy.position.y,
		enemy.position.z,
		1.0f
	};
}

void Enemy::draw(directX* directx, TexManager* tex, SpriteCommon* commonsp)
{
	if (Isarive == true)
	{
		if (enemy_move_Pat == 1)
		{
			directx->depthclear();
			object3D::objectcommon->object3DcommonBeginDraw();
			enemy.DrawModel_OnMaterial(enemy_view);
			bullet.draw();
		}
		else if (enemy_move_Pat == 2)
		{
			directx->depthclear();
			object3D::objectcommon->object3DcommonBeginDraw();
			enemy.DrawModel_OnMaterial(enemy_view2);
			bullet.draw();
		}
		else if (enemy_move_Pat == 3)
		{
			directx->depthclear();
			object3D::objectcommon->object3DcommonBeginDraw();
			enemy.DrawModel_OnMaterial(enemy_view3);
			bullet.draw();
		}

		if (IsRockon_draw == true)
		{
			directx->depthclear();
			commonsp->SpriteCommonBeginDraw(directx->cmdList.Get(), tex);
			Rock_Target.DrawSprite(directx->cmdList.Get(), tex, directx->dev.Get());
		}
	}
}
#pragma endregion
