#include"Player.h"

dxinput* Player::input = nullptr;

void Player::init(dxinput* input, TexManager* tex, directX* directx)
{
	this->input = input;

	target.anchorpoint = { 0.5f,0.5f };
	target.GenerateSprite("Target.png");

	Player_model = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	Player_object = new Object3d_FBX;
	Player_object->Initialize();
	Player_object->SetModel(Player_model);
	Player_object->SetPosition({ 0,5,+30 });
	Player_object->SetScale({ 1,1,1 });
	Player_object->setSpeed(2.0f);
	//Player_object->PlayAnimation();
	Player_object->setColor({ 0,1,1,1 });

	/*followcamera = new FollowCamera();

	followcamera->setFollowTarget(&Player_object->getPosition(), &Player_object->getRotation(), -30);

	Object3d_FBX::SetCamera(followcamera);

	followcamera->TargetObjectPos = &Player_object->getPosition();
	followcamera->TargetObjectAngle = &Player_object->getRotation();

	followcamera->Following();*/
	

	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		player_bullet[i].init(i + 1);
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		player_missiale[i].init(i + 21);
	}

	for (int i = 0; i < 10; i++)
	{
		hp[i].GenerateSprite("Player_HP.png");
		hp[i].size = { 40,80 };
		hp[i].position = { i * 30.0f + 10.0f,650.0f,0.0f };
		hp[i].SpriteTransferVertexBuffer(false);
	}

	player_collision.radius = 2.0f;

	HP = 10;
	Isarive = true;
}

void Player::Move()
{
	//前に進む
	if (input->push(DIK_W))
	{
		Player_object->addMoveFront(followcamera->getFrontVec());
	}
	//後に下がる
	if (input->push(DIK_S))
	{
		Player_object->addMoveBack(followcamera->getFrontVec());
	}

	if (input->mouse_p.x >= 1000)
	{
		objectRot.y += 0.7f;
		yow += 0.7f;
	}

	if (input->mouse_p.x <= 280)
	{
		objectRot.y -= 0.7f;
		yow -= 0.7f;
	}

	Player_object->SetRotation({ pitch,yow,roll });
	XMVECTOR matQ = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yow), XMConvertToRadians(roll));
	Player_object->addQRot(matQ);

	/*followcamera->TargetObjectPos = &Player_object->getPosition();
	followcamera->TargetObjectAngle = &Player_object->getRotation();

	followcamera->Following();

	followcamera->setFrontVec(0.5f);*/

	Player_object->Update();
	player_collision.center =
	{
		Player_object->getPosition().x,
		Player_object->getPosition().y,
		Player_object->getPosition().z,
		1.0f
	};
}

//敵とプレイヤー弾の当たり判定
//※プレイヤー更新処理の中には描かない
//※ゲームシーン内で書く
void Player::checkPlayerBullet(Enemy* enemy)
{
	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		//player_bullet[i].checkhit(enemy);
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		//player_missiale[i].checkhit();
	}
}

void Player::checkPlayerEnemy(Enemy* enemy)
{
	if (enemy->Isarive == false)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(player_collision, enemy->enemy_collision))
	{
		HP--;
		enemy->Isarive = false;
	}
}

//更新
void Player::update()
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
					player_bullet[i].set(Player_object->getPosition(),
						Player_object->screenToWorld({ input->mouse_position.x,input->mouse_position.y }));
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

		if (Target_count > 70)
		{
			target.rotation -= 5.0f;
			Isrockon = true;
		}
		else
		{
			target.rotation += 3.0f;
			//Rockon_count = 0;
			Isrockon = false;
		}
	}

	target.position = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };
	target.SpriteTransferVertexBuffer();
	target.SpriteUpdate();

	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		player_bullet[i].update();
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		player_missiale[i].update();
	}

	for (int i = 0; i < 10; i++)
	{
		hp[i].SpriteUpdate();
	}
}

void Player::reset()
{
	Isarive = true;
	HP = 10;

	up = 0;
	right = 0;
	objectRot = { 0,0,0 };
	Player_object->SetPosition({ 0,5,0 });
	pitch = 0.0f;
	yow = 0.0f;

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		player_missiale[i].P_enemy = nullptr;
		player_missiale[i].Isarive = false;
		player_missiale[i].IsTarget_set = false;
	}
}

//描画
void Player::draw_3d(directX* directx, TexManager* tex)
{
	if (Isarive == true)
	{
		Player_object->SetPipelineSimple(directx->cmdList.Get());
		Player_object->Draw(directx->cmdList.Get());

		for (int i = 0; i < MaxPlayerBulletNum; i++)
		{
			player_bullet[i].draw(directx);
		}

		for (int i = 0; i < MaxPlayerMissileNum; i++)
		{
			player_missiale[i].draw(directx);
		}
	}
}

void Player::draw_2d(directX* directx, TexManager* tex)
{
	if (!Isarive)
	{
		return;
	}

	target.DrawSprite(directx->cmdList.Get());

	for (int i = 0; i < HP; i++)
	{
		hp[i].DrawSprite(directx->cmdList.Get());
	}
}
