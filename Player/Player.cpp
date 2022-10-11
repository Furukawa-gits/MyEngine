#include"Player.h"

dxinput* Player::input = nullptr;

void Player::init(dxinput* input, directX* directx)
{
	this->input = input;

	target.anchorpoint = { 0.5f,0.5f };
	target.GenerateSprite("Target.png");

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

void Player::Move()
{
	//�O�ɐi�ݑ�����
	Player_object->addMoveFront(followcamera->getFrontVec());

	//�J�������[�N
	cameraMove();

	//�E�E��E�O�����̃x�N�g��
	XMFLOAT3 vSideAxis = getAxis(quaternion(unitX, qLocal));
	XMFLOAT3 vUpAxis = getAxis(quaternion(unitY, qLocal));
	XMFLOAT3 vForwordAxis = getAxis(quaternion(unitZ, qLocal));

	//���[���E�s�b�`�E���[�̉�]�p�x�����߂�
	Quaternion qRoll = quaternion(vForwordAxis, roll);
	Quaternion qPitch = quaternion(vSideAxis, pitch);
	Quaternion qYow = quaternion(vUpAxis, yow);

	//���Ԃɂ����Ă���
	qLocal = qRoll * qLocal;
	qLocal = qPitch * qLocal;
	qLocal = qYow * qLocal;

	//��]�s����Z�b�g
	Player_object->setRotMatrix(rotate(qLocal));

	//�Ǐ]
	followcamera->Following(vUpAxis, vForwordAxis, Player_object->getPosition());

	//�O�ɐi�ރx�N�g�����v�Z
	followcamera->setFrontVec(0.1f);

	//�I�u�W�F�N�g�̍X�V
	Player_object->Update();
	player_collision.center =
	{
		Player_object->getPosition().x,
		Player_object->getPosition().y,
		Player_object->getPosition().z,
		1.0f
	};
}

void Player::cameraMove()
{
	//���[��]
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

	//�e�����傫������K�p(������0�ɂȂ�̂Ō������Ă���)
	if (fabsf(yowRotateSpeedPositive) > fabsf(yowRotateSpeedNegative))
	{
		yow = yowRotateSpeedPositive;
	}
	else
	{
		yow = yowRotateSpeedNegative;
	}

	//�s�b�`��]
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

	//�e�����傫������K�p(������0�ɂȂ�̂Ō������Ă���)
	if (fabsf(pitchRotateSpeedPositive) > fabsf(pitchRotateSpeedNegative))
	{
		pitch = pitchRotateSpeedPositive;
	}
	else
	{
		pitch = pitchRotateSpeedNegative;
	}
}

//�G�ƃv���C���[�e�̓����蔻��
//���v���C���[�X�V�����̒��ɂ͕`���Ȃ�
//���Q�[���V�[�����ŏ���
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

//�X�V
void Player::update()
{
	if (Isarive == true)
	{
		//�ړ�
		Move();

		if (input->Mouse_LeftTriger())
		{
			//�e������
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

	//XMFLOAT3 test = Player_object->screenToWorld({ target.position.x,target.position.y });

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

//���Z�b�g
void Player::reset()
{
	Isarive = true;
	HP = 10;

	up = 0;
	right = 0;
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

//�`��
void Player::draw_3d(directX* directx)
{
	if (Isarive == true)
	{
		//�P�F�V�F�[�_���Z�b�g���ĕ`��
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

	target.DrawSprite(directx->cmdList.Get());

	for (int i = 0; i < HP; i++)
	{
		hp[i].DrawSprite(directx->cmdList.Get());
	}
}
