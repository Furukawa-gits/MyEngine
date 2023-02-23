#include"Player.h"

dxinput* Player::input = nullptr;
std::unique_ptr<Model> Player::playerModel = std::make_unique<Model>();


Player::Player()
{
	for (int i = 0; i < 10; i++)
	{
		HPUI->push_back(SingleSprite());
	}
}

Player::~Player()
{
	delete(playerObject);
	delete(followCamera);
	bulletsList.clear();
	missilesList.clear();

	bullet::staticDestroy();
	Missile::staticDestroy();
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

	damage.anchorpoint = { 0,0 };
	damage.size = { 1280,720 };
	damage.GenerateSprite("damage.png");

	for (int i = 0; i < 9; i++)
	{
		remainingMissileNum[i].GenerateSprite("remainingMissileNum.png");
	}

	playerModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("player"));

	playerObject = new Object3d_FBX;
	playerObject->Initialize();
	playerObject->SetModel(playerModel.get());
	playerObject->SetPosition({ 0,5,0 });
	playerObject->SetScale({ 1,1,1 });
	playerObject->SetScale({ 0.02f,0.02f,0.02f });
	playerObject->setSpeed(2.0f);
	//Player_object->PlayAnimation();
	playerObject->setColor({ 0,1,1,1 });

	followCamera = new FollowCamera();

	followCamera->setFollowTarget(playerObject->getPosition(), playerObject->getRotation(), -30);

	followCamera->SetEye({ 0,5,-10 });
	followCamera->SetTarget({ 0,5,0 });

	followCamera->setTargets(playerObject->getPosition(), playerObject->getRotation());

	Object3d_FBX::SetCamera(followCamera);

	bullet::staticInit();
	Missile::staticInit();

	//HP(vector)
	int i = 0;
	for (auto itr = HPUI->begin(); itr != HPUI->end(); itr++)
	{
		itr->GenerateSprite("Player_HP.png");
		itr->size = { 40,60 };
		itr->position = { i * 30.0f + 10.0f,650.0f,0.0f };
		itr->SpriteTransferVertexBuffer(false);
		i++;
	}

	playerCollision.radius = 2.0f;

	playerHP = 10;
	isArive = true;
}

//�ړ�
void Player::Move()
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	if (!isArive)
	{
		return;
	}

	//�I�u�W�F�N�g�̍X�V
	playerObject->Update();
	playerCollision.center =
	{
		playerObject->getPosition().x,
		playerObject->getPosition().y,
		playerObject->getPosition().z,
		1.0f
	};

	if (isStop)
	{
		return;
	}

	//�����őO�ɐi�ݑ�����
	playerObject->addMoveFront(followCamera->getFrontVec());

	//�}�E�X�ł̃J��������
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

	//�Ǐ]
	followCamera->Following(vUpAxis, vForwordAxis, playerObject->getPosition());

	//XMMATRIX�ɕϊ������N�H�[�^�j�I�����v���C���[�̉�]�s��ɃZ�b�g
	playerObject->setRotMatrix(rotate(qLocal));

	//�O�ւ̈ړ��ʂ��v�Z
	followCamera->setFrontVec(moveSpeed);
}

//�J�����ړ�
void Player::cameraMove()
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	//���[��]
	//�E
	if (targetFirst.position.x >= 1000)
	{
		cameraMoveCount++;
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
	//��
	if (targetFirst.position.x <= 280)
	{
		cameraMoveCount++;
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
	//��
	if (targetFirst.position.y >= 620)
	{
		cameraMoveCount++;
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
	//��
	if (targetFirst.position.y <= 100)
	{
		cameraMoveCount++;
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

void Player::playerClearMove()
{
	if (!isClearStaging)
	{
		return;
	}

	if (!isStagingSet)
	{
		return;
	}

	clearTime++;

	if (clearTime == maxClearTime)
	{
		isClearStaging = false;
	}
}

void Player::playerDeathMove()
{
	if (!isOverStaging)
	{
		return;
	}

	if (!isStagingSet)
	{
		return;
	}

	fallCount++;

	if (fallCount == maxFallCount)
	{
		isOverStaging = false;
		return;
	}

	//��]���Ȃ��痎���čs��
	fallRot.x = 0.05;
	fallRot.y = 0.05;
	fallRot.z = 0.05;
	playerObject->setRotMatrix(fallRot.x, fallRot.y, fallRot.z);

	playerObject->addMoveFront({ 0,-0.03,0 });

	fallScale.x -= 0.0000555f;
	fallScale.y -= 0.0000555f;
	fallScale.z -= 0.0000555f;
	playerObject->SetScale(fallScale);

	playerObject->Update();

	//�J��������
	followCamera->SetTarget(playerObject->getPosition());

	followCamera->Update();
}

void Player::setStaging(bool isclear)
{
	if (isStagingSet)
	{
		return;
	}

	XMFLOAT3 pos = playerObject->getPosition();

	followCamera->SetEye({ pos.x,pos.y + 20,pos.z - 10 });

	fallScale = playerObject->getScale();

	if (isclear)
	{
		isClearStaging = true;
		isOverStaging = false;
	}
	else
	{
		isClearStaging = false;
		isOverStaging = true;

		fallCount = 0;
	}

	isStagingSet = true;
}

//�X�V
void Player::update()
{
	//���񂾒e�͍폜
	bulletsList.remove_if([](std::unique_ptr<bullet>& newbullet)
		{
			return newbullet->isArive == false;
		});

	missilesList.remove_if([](std::unique_ptr<Missile>& newmissile)
		{
			return newmissile->isArive == false;
		});

	missilesList.remove_if([](std::unique_ptr<Missile>& newmissile)
		{
			return newmissile->enemyPointer == nullptr;
		});

	//�ړ�
	Move();

	//�^�[�Q�b�g�J�[�\���̏���
	targetUpdate();

	//�N���A���Q�[���I�[�o�[���o
	playerClearMove();
	playerDeathMove();

	if (isClearStaging || isOverStaging)
	{
		return;
	}

	//�U����A�������t���[���ŐH���Ȃ��悤�ɂ���
	if (isArmor)
	{
		armorTime++;

		if (armorTime % 20 == 0)
		{
			isInvisible *= -1;
		}

		if (armorTime >= maxArmorTime)
		{
			armorTime = 0;
			isArmor = false;
			isInvisible = -1;
		}
	}
	else
	{
		armorTime = 0;
	}

	//�_���[�W�\���X�v���C�g
	damage.SpriteTransferVertexBuffer();
	damage.SpriteUpdate();

	if (playerHP <= 0)
	{
		isArive = false;
		setStaging(false);
	}

	//�ʏ�e�̍X�V(���j�[�N���X�g)
	for (std::unique_ptr<bullet>& bullet : bulletsList)
	{
		bullet->update();
	}

	//�~�T�C���̍X�V(���j�[�N���X�g)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->update();
	}

	//HP�Q�[�W�̍X�V
	for (auto itr = HPUI->begin(); itr != HPUI->end(); itr++)
	{
		itr->SpriteUpdate();
	}
}

void Player::targetUpdate()
{
	if (isStop)
	{
		return;
	}

	if (!isArive)
	{
		return;
	}

	if (isClearStaging || isOverStaging)
	{
		return;
	}

	//�}�E�X�̈ړ��ʂ��^�[�Q�b�g�J�[�\���̍��W�ɉ��Z
	targetFirst.position.x += input->mouseMoveVecrocity.x;
	targetFirst.position.y += input->mouseMoveVecrocity.y;

	//�}�E�X�J�[�\���Œ�
	SetCursorPos(mouseOffsetX, mouseOffsetY);

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

	//�v���C���[�̃X�N���[�����W
	XMFLOAT2 PlayerScreenPosition = playerObject->worldToScleen();

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

	targetFirst.SpriteTransferVertexBuffer();
	targetFirst.SpriteUpdate();
	targetSecond.SpriteTransferVertexBuffer();
	targetSecond.SpriteUpdate();
	targetThird.SpriteTransferVertexBuffer();
	targetThird.SpriteUpdate();

	//���N���b�N�Œʏ�e
	if (input->Mouse_LeftTriger())
	{
		//���X�g��
		std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
		newBullet->init();
		newBullet->set(playerObject->getPosition(),
			playerObject->screenToWorld({ targetFirst.position.x,targetFirst.position.y }));

		bulletsList.push_back(std::move(newBullet));
	}

	//���b�N�I�����[�h�ɐ؂�ւ�
	if (input->Mouse_LeftPush())
	{
		targetCount++;
	}
	else
	{
		targetCount = 0;
	}

	if (targetCount > 70)
	{
		targetFirst.rotation -= 7.0f;
		targetSecond.rotation += 7.0f;
		targetThird.rotation -= 7.0f;
		isRockOn = true;
	}
	else
	{
		targetFirst.rotation += 4.0f;
		targetSecond.rotation -= 4.0f;
		targetThird.rotation += 4.0f;
		isRockOn = false;
	}

	//�~�T�C���c�e���̕\��
	for (int i = 0; i < 9; i++)
	{
		remainingMissileNum[i].anchorpoint = { 0,0 };
		remainingMissileNum[i].position = { 640 + (remainingMissileNum[i].size.x / 2),360,0 };
		remainingMissileNum[i].texLeftTop = { (float)i * 100,0 };
		remainingMissileNum[i].texSize = { 100,100 };
		remainingMissileNum[i].size = { 70,70 };

		remainingMissileNum[i].SpriteTransferVertexBuffer(true);
		remainingMissileNum[i].SpriteUpdate();
	}
}

void Player::addMissile(Enemy* enemy)
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	//�~�T�C���ǉ�
	std::unique_ptr<Missile> newMissile = std::make_unique<Missile>();
	newMissile->init();
	newMissile->setPenemy(enemy);
	newMissile->start(playerObject->getPosition());

	missilesList.push_back(std::move(newMissile));
}

//���Z�b�g
void Player::reset()
{
	isArive = true;
	isStagingSet = false;
	playerHP = 10;
	isInvisible = -1;
	playerObject->SetPosition({ 0,5,0 });
	playerObject->SetScale({ 0.02f,0.02f,0.02f });
	playerObject->setRotMatrix(XMMatrixIdentity());
	roll = 0.0f;
	pitch = 0.0f;
	yow = 0.0f;
	qLocal = quaternion();
	followCamera = new FollowCamera();
	followCamera->setFollowTarget(playerObject->getPosition(), playerObject->getRotation(), -30);
	followCamera->SetEye({ 0,5,-10 });
	followCamera->SetTarget({ 0,5,0 });
	followCamera->setTargets(playerObject->getPosition(), playerObject->getRotation());

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

	//�Ǐ]
	followCamera->Following(vUpAxis, vForwordAxis, playerObject->getPosition());

	targetFirst.position = { (float)mouseOffsetX,(float)mouseOffsetY ,0 };
	SetCursorPos(mouseOffsetX, mouseOffsetY);
	cameraMoveCount = 0;
	Object3d_FBX::SetCamera(followCamera);
}

//�`��
void Player::draw3D(directX* directx)
{
	if (!isArive)
	{
		//return;
	}

	//�v���C���[�{�̂̕`��
	if (isInvisible == -1)
	{
		playerObject->Draw(directx->cmdList.Get());
	}

	//�ʏ�e�̕`��(���j�[�N���X�g)
	for (std::unique_ptr<bullet>& bullet : bulletsList)
	{
		bullet->draw(directx);
	}
	//�~�T�C���̍X�V(���j�[�N���X�g)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->draw(directx);
	}
}

void Player::draw2D(directX* directx, int targetnum)
{
	if (!isArive)
	{
		return;
	}

	if (isStop)
	{
		return;
	}

	targetThird.DrawSprite(directx->cmdList.Get());
	targetSecond.DrawSprite(directx->cmdList.Get());
	targetFirst.DrawSprite(directx->cmdList.Get());

	if (isArmor)
	{
		damage.DrawSprite(directx->cmdList.Get());
	}

	if (isRockOn)
	{
		remainingMissileNum[MaxPlayerMissileNum - targetnum].DrawSprite(directx->cmdList.Get());
	}

	int i = 0;
	for (auto itr = HPUI->begin(); itr != HPUI->end(); itr++)
	{
		if (i >= playerHP)
		{
			break;
		}
		itr->DrawSprite(directx->cmdList.Get());
		i++;
	}
}
