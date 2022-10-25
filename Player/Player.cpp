#include"Player.h"

dxinput* Player::input = nullptr;

Player::Player()
{
}

Player::~Player()
{
	delete(Player_object);
	delete(followcamera);
}

void Player::init(dxinput* input, directX* directx)
{
	this->input = input;

	targetFirst.anchorpoint = { 0.5f,0.5f };
	targetFirst.size = { 40,40 };
	targetFirst.GenerateSprite("Target.png");

	targetSecond.anchorpoint = { 0.5f,0.5f };
	targetSecond.size = { 60,60 };
	targetSecond.GenerateSprite("Target.png");

	targetThird.anchorpoint = { 0.5f,0.5f };
	targetThird.size = { 80,80 };
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
		playerBullet[i].init(i + 1);
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		playerMissiale[i].init(i + 21);
	}

	for (int i = 0; i < 10; i++)
	{
		hitPointUI[i].GenerateSprite("Player_HP.png");
		hitPointUI[i].size = { 40,80 };
		hitPointUI[i].position = { i * 30.0f + 10.0f,650.0f,0.0f };
		hitPointUI[i].SpriteTransferVertexBuffer(false);
	}

	player_collision.radius = 2.0f;

	playerHP = 10;
	isArive = true;
}

//�ړ�
void Player::Move()
{
	//�����őO�ɐi�ݑ�����
	Player_object->addMoveFront(followcamera->getFrontVec());

	//�J�������[�N
	cameraMove();

	//�u�v���C���[���猩�āv�E�E��E�O�����̃x�N�g��
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

	//XMMATRIX�ɕϊ������N�H�[�^�j�I�����v���C���[�̉�]�s��ɃZ�b�g
	Player_object->setRotMatrix(rotate(qLocal));

	//�Ǐ]
	followcamera->Following(vUpAxis, vForwordAxis, Player_object->getPosition());

	//�O�ւ̈ړ��ʂ��v�Z
	followcamera->setFrontVec(moveSpeed);

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

//�J�����ړ�
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
		if (pitchRotateSpeedPositive > -limitRotateSpeed)
		{
			pitchRotateSpeedPositive -= addRotateSpeed;
		}
	}
	else
	{
		if (pitchRotateSpeedPositive < 0)
		{
			pitchRotateSpeedPositive -= subRotateSpeed;
		}
	}

	if (input->mouse_p.y <= 100)
	{
		if (pitchRotateSpeedNegative < limitRotateSpeed)
		{
			pitchRotateSpeedNegative += addRotateSpeed;
		}
	}
	else
	{
		if (pitchRotateSpeedNegative > 0)
		{
			pitchRotateSpeedNegative += subRotateSpeed;
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

	if (Collision::CheckSphere2Sphere(player_collision, enemy->enemyCollision))
	{
		playerHP--;
		enemy->Isarive = false;
	}
}

//�X�V
void Player::update()
{
	if (!isArive)
	{
		return;
	}

	//�ړ�
	Move();

	//�^�[�Q�b�g�J�[�\���̏���
	targetUpdate();

	if (playerHP <= 0)
	{
		isArive = false;
	}

	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		playerBullet[i].update();
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		playerMissiale[i].update();
	}

	for (int i = 0; i < 10; i++)
	{
		hitPointUI[i].SpriteUpdate();
	}
}

void Player::targetUpdate()
{
	//���N���b�N�Œʏ�e
	if (input->Mouse_LeftTriger())
	{
		//�e������
		for (int i = 0; i < MaxPlayerBulletNum; i++)
		{
			if (playerBullet[i].isArive == false)
			{
				playerBullet[i].set(Player_object->getPosition(),
					Player_object->screenToWorld({ input->mouse_position.x,input->mouse_position.y }));
				break;
			}
		}
	}

	//���b�N�I�����[�h�ɐ؂�ւ�
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
		targetFirst.rotation -= 7.0f;
		targetSecond.rotation += 7.0f;
		targetThird.rotation -= 7.0f;
		Isrockon = true;
	}
	else
	{
		targetFirst.rotation += 4.0f;
		targetSecond.rotation -= 4.0f;
		targetThird.rotation += 4.0f;
		Isrockon = false;
	}

	targetFirst.position = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };

	//�^�[�Q�b�g�J�[�\������O�ɂ����Ȃ��悤�ɐ���
	if (targetFirst.position.x <= 29)
	{
		targetFirst.position.x = 29;
	}
	if (targetFirst.position.x >= 1251)
	{
		targetFirst.position.x = 1251;
	}
	if (targetFirst.position.y <= 29)
	{
		targetFirst.position.y = 29;
	}
	if (targetFirst.position.y >= 691)
	{
		targetFirst.position.y = 691;
	}

	targetFirst.SpriteTransferVertexBuffer();
	targetFirst.SpriteUpdate();

	//�v���C���[�̃X�N���[�����W
	XMFLOAT2 PlayerScreenPosition = Player_object->worldToScleen();

	XMFLOAT2 secondTargetPos =
	{
		PlayerScreenPosition.x + (targetFirst.position.x - PlayerScreenPosition.x) * 0.666f,
		PlayerScreenPosition.y + (targetFirst.position.y - PlayerScreenPosition.y) * 0.666f,
	};

	targetSecond.position =
	{
		secondTargetPos.x,
		secondTargetPos.y,
		0.0f
	};
	targetSecond.SpriteTransferVertexBuffer();
	targetSecond.SpriteUpdate();

	XMFLOAT2 thirdTargetPos =
	{
		PlayerScreenPosition.x + (targetFirst.position.x - PlayerScreenPosition.x) * 0.333f,
		PlayerScreenPosition.y + (targetFirst.position.y - PlayerScreenPosition.y) * 0.333f,
	};

	targetThird.position =
	{
		thirdTargetPos.x,
		thirdTargetPos.y,
		0.0f
	};
	targetThird.SpriteTransferVertexBuffer();
	targetThird.SpriteUpdate();
}

//���Z�b�g
void Player::reset()
{
	isArive = true;
	playerHP = 10;

	Player_object->SetPosition({ 0,5,0 });
	pitch = 0.0f;
	yow = 0.0f;

	qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		playerMissiale[i].P_enemy = nullptr;
		playerMissiale[i].Isarive = false;
		playerMissiale[i].IsTarget_set = false;
	}
}

//�`��
void Player::draw3D(directX* directx)
{
	if (!isArive)
	{
		return;
	}

	//�P�F�V�F�[�_���Z�b�g���ĕ`��
	Player_object->SetPipelineSimple(directx->cmdList.Get());
	Player_object->Draw(directx->cmdList.Get());

	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		playerBullet[i].draw(directx);
	}

	for (int i = 0; i < MaxPlayerMissileNum; i++)
	{
		playerMissiale[i].draw(directx);
	}
}

void Player::draw2D(directX* directx)
{
	if (!isArive)
	{
		return;
	}

	targetThird.DrawSprite(directx->cmdList.Get());
	targetSecond.DrawSprite(directx->cmdList.Get());
	targetFirst.DrawSprite(directx->cmdList.Get());

	for (int i = 0; i < playerHP; i++)
	{
		hitPointUI[i].DrawSprite(directx->cmdList.Get());
	}
}
