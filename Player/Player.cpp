#include"Player.h"

dxinput* Player::input = nullptr;

void Player::init(dxinput* input, TexManager* tex, directX* directx)
{
	this->input = input;

	player_view = object3D_obj::objectcommon->ReturnModelViewes("RS2_Shooting.obj", 0);

	old_player_view = player_view;

	Player_object.Init3d(0);
	Player_object.scale = { 3.0f,3.0f,3.0f };

	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		player_bullet[i].init(i + 1);
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		player_missiale[i].init(i + 21);
	}

	Isarive = true;

	//Target.GenerateSprite(object3D_obj::directx->dev.Get(), 2, tex, false, false, false, false);

	player_collision.radius = 4.0f;

	for (int i = 0; i < 10; i++)
	{
		//hp[i].GenerateSprite(object3D_obj::directx->dev.Get(),13, tex, false, false, false, false);
		hp[i].anchorpoint = { 0.0f,0.0f };
		hp[i].position = { (i * 30.0f) + 30.0f,670.0f,0.0f };
		hp[i].size = { 40.0f,40.0f };
		//hp[i].SpriteTransferVertexBuffer(tex, false);
	}
}

void Player::Move()
{
	if (input->push(DIK_D) && Player_object.position.x <= Move_limit)
	{
		Player_object.position.x += 1.0f;
	}

	if (input->push(DIK_A) && Player_object.position.x >= -Move_limit)
	{
		Player_object.position.x -= 1.0f;
	}

	if (input->push(DIK_W) && Player_object.position.y <= Move_limit)
	{
		Player_object.position.y += 1.0f;
	}

	if (input->push(DIK_S) && Player_object.position.y >= -Move_limit)
	{
		Player_object.position.y -= 1.0f;
	}
}

//敵とプレイヤー弾の当たり判定			※プレイヤー更新処理の中には描かない		※ゲームシーン内で書く
void Player::checkplayerbullet(Enemy* enemy)
{
	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		player_bullet[i].checkhit(enemy);
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		player_missiale[i].checkhit();
	}
}

//敵の弾との当たり判定
void Player::checkenemybullet(enemy_bullet* bullet)
{
	if (Collision::CheckSphere2Sphere(player_collision, bullet->bullet_collision) && bullet->Isarive == true && Isarive == true)
	{
		HP--;

		if (HP <= 0)
		{
			Isarive = false;
		}

		bullet->Isarive = false;
	}
}

//敵をロックオン
void Player::checkrockon(Enemy& enemy)
{
	if (Isrockon == true)
	{
		if (Target.position.x <= enemy.enemy_sc_pos.m128_f32[0] + 40.0f && Target.position.x >= enemy.enemy_sc_pos.m128_f32[0] - 40.0f)
		{
			if (Target.position.y <= enemy.enemy_sc_pos.m128_f32[1] + 40.0f && Target.position.y >= enemy.enemy_sc_pos.m128_f32[1] - 40.0f)
			{
				if (enemy.Isarive == true && enemy.IsRockon_draw == false && enemy.enemy.position.z <= 200 && Rockon_count < MaxPlayerMissileNum)
				{
					enemy.IsRockon_draw = true;
					Rockon_count++;

					for (int i = 0; i < MaxPlayerMissileNum; i++)
					{
						if (player_missiale[i].IsTarget_set == false)
						{
							player_missiale[i].setPenemy(&enemy);
							break;
						}
					}
				}
			}
		}
	}
}

//更新
void Player::update(SpriteCommon* commonsp)
{
	if (Isarive == true)
	{
		//移動
		Move();

		if (input->Mouse_LeftTriger())
		{
			//弾を撃つ
			for (int i = 0; i < MaxPlayerBulletNum; i++)
			{
				if (player_bullet[i].Isarive == false)
				{
					player_bullet[i].set(Player_object.position, input->mouse_position);
					break;
				}
			}
		}

		if (HP <= 0)
		{
			Isarive = false;
		}

		if (input->Mouse_LeftPush())
		{
			Target_count++;
		}
		else
		{
			Target_count = 0;
		}

		if (Target_count > 40)
		{
			Target.rotation -= 7.0f;
			Isrockon = true;
		}
		else
		{
			Target.rotation += 3.0f;
			//Rockon_count = 0;
			Isrockon = false;
		}

		if (input->Mouse_LeftRelease())
		{
			for (int i = 0; i < Rockon_count; i++)
			{
				player_missiale[i].start(Player_object.position);
			}
			Rockon_count = 0;
		}
	}

	Player_object.Update3d(player_view);

	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		player_bullet[i].update();
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		player_missiale[i].update();
	}

	Target.position = input->mouse_position;
	//Target.SpriteUpdate(*commonsp);

	for (int i = 0; i < 10; i++)
	{
		//hp[i].SpriteUpdate(*commonsp);
	}

	player_collision.center = {
		Player_object.position.x,
		Player_object.position.y,
		Player_object.position.z,
		1.0f
	};
}

//描画
void Player::draw(directX* directx, TexManager* tex, SpriteCommon* commonsp)
{
	if (Isarive == true)
	{
		object3D_obj::objectcommon->object3DcommonBeginDraw();

		for (int i = 0; i < MaxPlayerBulletNum; i++)
		{
			player_bullet[i].draw();
		}

		for (int i = 0; i < MaxPlayerMissileNum; i++)
		{
			player_missiale[i].draw();
		}

		Player_object.DrawModel_OnMaterial(player_view);

		directx->depthclear();
		commonsp->SpriteCommonBeginDraw(directx->cmdList.Get(), tex);
		//Target.DrawSprite(directx->cmdList.Get(), tex);

		for (int i = 0; i < HP; i++)
		{
			//hp[i].DrawSprite(directx->cmdList.Get(), tex);
		}
	}
}
