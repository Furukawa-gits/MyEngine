#include"Player.h"

dxinput* Player::input = nullptr;

void Player::init(dxinput* input, directX* directx)
{
	this->input = input;

	targetFirst.anchorpoint = { 0.5f,0.5f };
	targetFirst.size = { 50,50 };
	targetFirst.GenerateSprite("Target.png");

	targetSecond.anchorpoint = { 0.5f,0.5f };
	targetSecond.size = { 75,75 };
	targetSecond.GenerateSprite("Target.png");

	targetThird.anchorpoint = { 0.5f,0.5f };
	targetThird.size = { 100,100 };
	targetThird.GenerateSprite("Target.png");

	Player_model = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	Player_object = new Object3d_FBX;
	Player_object->Initialize();
	Player_object->SetModel(Player_model);
	Player_object->SetPosition({ 0,5,0 });
	Player_object->SetScale({ 1,1,1 });
	Player_object->setSpeed(2.0f);
	//Player_object->PlayAnimation();
	Player_object->setColor({ 0,1,1,1 });

	followcamera = new FollowCamera();

	followcamera->setFollowTarget(Player_object->getPosition(), Player_object->getRotation(), -30);

	followcamera->SetEye({ 0,5,-10 });
	followcamera->SetTarget({ 0,5,0 });

	followcamera->setTargets(Player_object->getPosition(), Player_object->getRotation());

	Object3d_FBX::SetCamera(followcamera);

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

//移動
void Player::Move()
{
	//自動で前に進み続ける
	//Player_object->addMoveFront(followcamera->getFrontVec());

	//カメラワーク
	cameraMove();

	//「プレイヤーから見て」右・上・前方向のベクトル
	XMFLOAT3 vSideAxis = getAxis(quaternion(unitX, qLocal));
	XMFLOAT3 vUpAxis = getAxis(quaternion(unitY, qLocal));
	XMFLOAT3 vForwordAxis = getAxis(quaternion(unitZ, qLocal));

	//ロール・ピッチ・ヨーの回転角度を求める
	Quaternion qRoll = quaternion(vForwordAxis, roll);
	Quaternion qPitch = quaternion(vSideAxis, pitch);
	Quaternion qYow = quaternion(vUpAxis, yow);

	//順番にかけていく
	qLocal = qRoll * qLocal;
	qLocal = qPitch * qLocal;
	qLocal = qYow * qLocal;

	//XMMATRIXに変換したクォータニオンをプレイヤーの回転行列にセット
	Player_object->setRotMatrix(rotate(qLocal));

	//追従
	followcamera->Following(vUpAxis, vForwordAxis, Player_object->getPosition());

	//前への移動量を計算
	followcamera->setFrontVec(moveSpeed);

	//オブジェクトの更新
	Player_object->Update();
	player_collision.center =
	{
		Player_object->getPosition().x,
		Player_object->getPosition().y,
		Player_object->getPosition().z,
		1.0f
	};
}

//カメラ移動
void Player::cameraMove()
{
	//ヨー回転
	if (input->mouse_p.x >= 1000)
	{
		if (yowRotateSpeedPositive < limitRotateSpeed)
		{
			yowRotateSpeedPositive += addRotateSpeed;
		}
	}
	else
	{
		if (yowRotateSpeedPositive > 0)
		{
			yowRotateSpeedPositive += subRotateSpeed;
		}
	}

	if (input->mouse_p.x <= 280)
	{
		if (yowRotateSpeedNegative > -limitRotateSpeed)
		{
			yowRotateSpeedNegative -= addRotateSpeed;
		}
	}
	else
	{
		if (yowRotateSpeedNegative < 0)
		{
			yowRotateSpeedNegative -= subRotateSpeed;
		}
	}

	//影響が大きい方を適用(いずれ0になるので減速していく)
	if (fabsf(yowRotateSpeedPositive) > fabsf(yowRotateSpeedNegative))
	{
		yow = yowRotateSpeedPositive;
	}
	else
	{
		yow = yowRotateSpeedNegative;
	}

	//ピッチ回転
	if (input->mouse_p.y >= 620)
	{
		if (pitchRotateSpeedPositive < limitRotateSpeed)
		{
			pitchRotateSpeedPositive += addRotateSpeed;
		}
	}
	else
	{
		if (pitchRotateSpeedPositive > 0)
		{
			pitchRotateSpeedPositive += subRotateSpeed;
		}
	}

	if (input->mouse_p.y <= 100)
	{
		if (pitchRotateSpeedNegative > -limitRotateSpeed)
		{
			pitchRotateSpeedNegative -= addRotateSpeed;
		}
	}
	else
	{
		if (pitchRotateSpeedNegative < 0)
		{
			pitchRotateSpeedNegative -= subRotateSpeed;
		}
	}

	//影響が大きい方を適用(いずれ0になるので減速していく)
	if (fabsf(pitchRotateSpeedPositive) > fabsf(pitchRotateSpeedNegative))
	{
		pitch = pitchRotateSpeedPositive;
	}
	else
	{
		pitch = pitchRotateSpeedNegative;
	}
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
	if (!Isarive)
	{
		return;
	}

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
		targetFirst.rotation -= 5.0f;
		targetSecond.rotation += 5.0f;
		Isrockon = true;
	}
	else
	{
		targetFirst.rotation += 3.0f;
		targetSecond.rotation -= 3.0f;
		Isrockon = false;
	}

	targetFirst.position = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };
	targetFirst.SpriteTransferVertexBuffer();
	targetFirst.SpriteUpdate();

	//ターゲットカーソルのワールド座標
	XMFLOAT3 targetWorldPosition = Player_object->screenToWorld({ targetFirst.position.x,targetFirst.position.y });

	XMFLOAT3 halfPlayerToTarget =
	{
		(targetWorldPosition.x - Player_object->getPosition().x) / 100 / 2,
		(targetWorldPosition.y - Player_object->getPosition().y) / 100 / 2,
		(targetWorldPosition.z - Player_object->getPosition().z) / 100 / 2
	};

	targetSecond.position =
	{
		Player_object->worldToScleenSpecifyPosition(halfPlayerToTarget).x,
		Player_object->worldToScleenSpecifyPosition(halfPlayerToTarget).y,
		0.0f
	};
	targetSecond.SpriteTransferVertexBuffer();
	targetSecond.SpriteUpdate();

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

//リセット
void Player::reset()
{
	Isarive = true;
	HP = 10;

	Player_object->SetPosition({ 0,5,0 });
	pitch = 0.0f;
	yow = 0.0f;

	qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		player_missiale[i].P_enemy = nullptr;
		player_missiale[i].Isarive = false;
		player_missiale[i].IsTarget_set = false;
	}
}

//描画
void Player::draw_3d(directX* directx)
{
	if (Isarive == true)
	{
		//単色シェーダをセットして描画
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

void Player::draw_2d(directX* directx)
{
	if (!Isarive)
	{
		return;
	}

	targetSecond.DrawSprite(directx->cmdList.Get());
	targetFirst.DrawSprite(directx->cmdList.Get());

	for (int i = 0; i < HP; i++)
	{
		hp[i].DrawSprite(directx->cmdList.Get());
	}
}
