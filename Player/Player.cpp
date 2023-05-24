#include"Player.h"

dxinput* Player::input = nullptr;
std::unique_ptr<Model> Player::playerModel = std::make_unique<Model>();


Player::Player()
{
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

	//�^�[�Q�b�g�A�C�R��
	targetFirst.anchorpoint = { 0.5f,0.5f };
	targetFirst.size = { 40,40 };
	targetFirst.generateSprite("Target.png");

	targetSecond.anchorpoint = { 0.5f,0.5f };
	targetSecond.size = { 60,60 };
	targetSecond.generateSprite("Target.png");

	targetThird.anchorpoint = { 0.5f,0.5f };
	targetThird.size = { 80,80 };
	targetThird.generateSprite("Target.png");

	//���b�N�I�����[�h�̃Q�[�W
	rockonGauge[0].anchorpoint = { 0.5f,0.5f };
	rockonGauge[0].size = { 0,20 };
	rockonGauge[0].position = { 640,420,0 };
	rockonGauge[0].generateSprite("playerHPGauge.png");

	rockonGauge[1].anchorpoint = { 0.5f,0.5f };
	rockonGauge[1].size = { 0,10 };
	rockonGauge[1].position = { 640,420,0 };
	rockonGauge[1].generateSprite("boostGauge.png");

	//�_���[�W�G�t�F�N�g
	damage.size = { 1280,720 };
	damage.generateSprite("damage.png");

	//HP�Q�[�W
	HPGaugeBar.size = { 50,20 };
	HPGaugeBar.position = { 20,640,0 };
	HPGaugeBar.generateSprite("playerHPGauge.png");

	//HP�o�[�̌x��
	dangarHPGaugeBar.size = { (float)maxHP * 40,20 };
	dangarHPGaugeBar.position = { 20,640,0 };
	dangarHPGaugeBar.generateSprite("bossHPGauge.png");

	//�u�[�X�g�Q�[�W
	boostGaugeBar.size = { 50,20 };
	boostGaugeBar.position = { 20,590,0 };
	boostGaugeBar.generateSprite("boostGauge.png");

	//��L�Q�[�W�̃t���[��
	gaugeFrame.size = { 500,120 };
	gaugeFrame.position = { 0,550,0 };
	gaugeFrame.generateSprite("gaugeFrame.png");

	//�G���A�O�x��
	outAreaCaution.anchorpoint = { 0.5f,0.5f };
	outAreaCaution.size = { 645,150 };
	outAreaCaution.position = { 640,360,0 };
	outAreaCaution.generateSprite("outAreaCaution.png");

	//�~�j�}�b�v�A�C�R��
	miniMapPlayer.anchorpoint = { 0.5f,0.5f };
	miniMapPlayer.size = { 4,4 };
	miniMapPlayer.generateSprite("playerHPGauge.png");

	//�~�T�C���̎c��e��
	for (int i = 0; i < 8; i++)
	{
		remainingMissileNum[i].anchorpoint = { 0.5f,0.5f };
		remainingMissileNum[i].size = { 200,200 };
		remainingMissileNum[i].rotation = (float)i * 45;
		remainingMissileNum[i].generateSprite("homingGauge.png");
	}

	remainingMissileNum[8].generateSprite("remainingMissileNum.png");

	playerModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("player"));

	playerObject = new object3dFBX;
	playerObject->initialize();
	playerObject->SetModel(playerModel.get());
	playerObject->SetPosition({ 0,5,0 });
	playerObject->SetScale({ 1,1,1 });
	playerObject->SetScale({ 0.02f,0.02f,0.02f });
	playerObject->setSpeed(2.0f);
	playerObject->setColor({ 1,1,1,1 });

	followCamera = new FollowCamera();

	followCamera->setFollowTarget(playerObject->getPosition(), playerObject->getRotation(), -30);

	followCamera->SetEye({ 0,5,-10 });
	followCamera->SetTarget({ 0,5,0 });

	followCamera->setTargets(playerObject->getPosition(), playerObject->getRotation());

	object3dFBX::SetCamera(followCamera);

	bullet::staticInit();
	Missile::staticInit();

	playerCollision.radius = 2.0f;

	playerHP = maxHP;
	isAlive = true;
}

//�ړ�*
void Player::move()
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	if (!isAlive)
	{
		return;
	}

	if (isStop)
	{
		return;
	}

	moveParticlesCount++;

	if (moveParticlesCount % 10 == 0)
	{
		SingleParticle newparticle;
		newparticle.generate();
		newparticle.set(10, playerObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 2.0, 0.0);
		newparticle.color = { 0.5f,0.5f,0.5f,0.2f };
		newparticle.isAddBlend = true;

		particleManagerOnTime::addParticle(newparticle, "effect1.png");
	}

	//�u�[�X�g(��)
	boostMove();

	//�I�u�W�F�N�g�̍X�V
	playerObject->setRotMatrix(XMMatrixIdentity());
	playerCollision.center =
	{
		playerObject->getPosition().x,
		playerObject->getPosition().y,
		playerObject->getPosition().z,
		1.0f
	};

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

	if (!isBoost)
	{
		roll = 0.0f;
	}

	//XMMATRIX�ɕϊ������N�H�[�^�j�I�����v���C���[�̉�]�s��ɃZ�b�g
	playerObject->setRotMatrix(rotate(qLocal));

	playerObject->updata();

	//�O�ւ̈ړ��ʂ��v�Z
	followCamera->setFrontVec(moveSpeed);
}

void Player::outArea()
{
	//�v���C���[�̍��W
	XMFLOAT3 ppos = playerObject->getPosition();

	//���_����̋������v�Z
	lengthForPlayerPosition = sqrtf(powf(ppos.x, 2) + powf(ppos.y, 2) + powf(ppos.z, 2));

	//�V���̊Oor�n�ʂɋ߂��Ȃ�x��
	if (lengthForPlayerPosition >= 400 || ppos.y <= groundPosition.y + 30)
	{
		isOutArea = true;
	}
	else
	{
		//�G���A���ɖ߂����烊�Z�b�g
		isOutArea = false;
		isCautionDraw = false;
		outAreaCount = 0;
	}

	//�G���A�O�ɂ��鎞�Ԃ��J�E���g
	if (isOutArea)
	{
		outAreaCount++;

		if (outAreaCount % 10 == 0)
		{
			isCautionDraw = !isCautionDraw;
		}
	}

	//�x���𖳎���������ꍇ�͎��S
	if (outAreaCount >= 700)
	{
		playerHP = 0;
	}

	//�x���X�v���C�g�̍X�V
	outAreaCaution.spriteUpdata();
}

void Player::boostMove()
{
	if (!isBoostTutorial)
	{
		return;
	}

	if (boostGauge < maxBoostGauge)
	{
		boostGauge += 0.5f;
	}
	else
	{
		boostGauge = maxBoostGauge;
	}

	if (moveSpeed > defaultMoveSpeed)
	{
		moveSpeed -= 0.08f;
	}
	else if (isBoost)
	{
		moveSpeed = defaultMoveSpeed;
		isBoost = false;
	}

	if (boostGauge < 100)
	{
		return;
	}

	if (input->Triger(DIK_SPACE) && !isBoost && moveSpeed == defaultMoveSpeed)
	{
		moveSpeed = boostMoveSpeed;
		boostGauge -= 100;
		yow = 0.0f;
		pitch = 0.0f;
		boostCount++;
		isBoost = true;
	}
}

//�J�����ړ�
void Player::cameraMove()
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	if (isBoost)
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

#pragma region �����p�[�e�B�N��
	if (fallCount % 30 == 0)
	{
		SingleParticle newparticle;
		newparticle.generate();

		XMFLOAT3 smokePos = playerObject->getPosition();

		smokePos =
		{
			smokePos.x + (float)(rand() % 6 - 3),
			smokePos.y + (float)(rand() % 6 - 3),
			smokePos.z + (float)(rand() % 6 - 3)
		};
		newparticle.set(100, smokePos, { 0,0,0 }, { 0,0,0 }, 5.0, 2.0);

		particleManagerOnTime::addParticle(newparticle, "smoke.png");
	}
#pragma endregion �����p�[�e�B�N��

	//��]���Ȃ��痎���čs��
	fallRot.x = 0.05;
	fallRot.y = 0.05;
	fallRot.z = 0.05;
	playerObject->setRotMatrix(fallRot.x, fallRot.y, fallRot.z);

	//playerObject->addMoveFront({ 0,-0.03,0 });

	fallScale.x -= 0.0000555f;
	fallScale.y -= 0.0000555f;
	fallScale.z -= 0.0000555f;
	playerObject->SetScale(fallScale);

	playerObject->updata();

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
void Player::updata()
{
	//���񂾒e�͍폜
	bulletsList.remove_if([](std::unique_ptr<bullet>& newbullet)
		{
			return newbullet->isAlive == false;
		});

	missilesList.remove_if([](std::unique_ptr<Missile>& newmissile)
		{
			return newmissile->isAlive == false;
		});

	missilesList.remove_if([](std::unique_ptr<Missile>& newmissile)
		{
			return newmissile->enemyPointer == nullptr;
		});

	//�ړ�
	move();

	//�G���A�O����
	outArea();

	//�^�[�Q�b�g�J�[�\���̏���
	targetUpdata();

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

		if (armorTime % 10 == 0)
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
	damage.spriteUpdata();

	//HP�Q�[�W�̍X�V
	HPGaugeBar.size = { (float)playerHP * 40,20 };
	HPGaugeBar.spriteUpdata();

	boostGaugeBar.size = { (float)boostGauge / 1.5f,20 };
	boostGaugeBar.spriteUpdata();

	dangarHPGaugeBar.spriteUpdata();

	gaugeFrame.spriteUpdata();

	//HP��4�����Ȃ�HP�o�[�_��
	if (playerHP < 4)
	{
		cautionHPCount++;

		if (cautionHPCount % 7 == 0)
		{
			isDangerHP = !isDangerHP;
		}
	}
	else
	{
		cautionHPCount = 0;
		isDangerHP = false;
	}

	//�~�j�}�b�v�A�C�R���X�V
	XMFLOAT3 playerPosition = playerObject->getPosition();

	//�ォ�猩���ʒu�Ȃ̂� X�EZ���W
	XMFLOAT3 minimapPosition =
	{
		(playerPosition.x * 0.21f) + Enemy::miniMapPosition.x,
		(playerPosition.z * 0.21f) + Enemy::miniMapPosition.x,
		0
	};

	miniMapPlayer.position = minimapPosition;
	miniMapPlayer.spriteUpdata();

	if (playerHP <= 0)
	{
		isAlive = false;
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
}

void Player::targetUpdata()
{
	if (isStop)
	{
		return;
	}

	if (!isAlive)
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

	//�����[�X���̂݃}�E�X�J�[�\���Œ�
#ifdef _DEBUG
	SetCursorPos(mouseOffsetX, mouseOffsetY);
#else
	SetCursorPos(mouseOffsetX, mouseOffsetY);
#endif // DEBUG

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

	targetFirst.spriteUpdata();
	targetSecond.spriteUpdata();
	targetThird.spriteUpdata();

	//���N���b�N�Œʏ�e
	if (input->Mouse_LeftTriger() && isNormalShot)
	{
		//���X�g��
		std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
		newBullet->init();
		newBullet->set(playerObject->getPosition(),
			playerObject->screenToWorld({ targetFirst.position.x,targetFirst.position.y }));

		bulletsList.push_back(std::move(newBullet));
		normalShotCount++;
	}

	//���b�N�I�����[�h�ɐ؂�ւ�
	if (input->Mouse_LeftPush() && isHomingMissile)
	{
		targetCount++;
	}
	else
	{
		targetCount = 0;
	}

	rockonGauge[0].size.x = (targetCount - 5) * 3.5f;
	rockonGauge[1].size.x = (targetCount - 5) * 3.5f;

	rockonGauge[0].spriteUpdata();
	rockonGauge[1].spriteUpdata();

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
	for (int i = 0; i < 8; i++)
	{
		remainingMissileNum[i].position = { 640,360,0 };
		remainingMissileNum[i].spriteUpdata();
	}

	remainingMissileNum[8].anchorpoint = { 0,0 };
	remainingMissileNum[8].position = { 640 + (remainingMissileNum[8].size.x / 2),360,0 };
	remainingMissileNum[8].texLeftTop = { 0,0 };
	remainingMissileNum[8].texSize = { 100,100 };
	remainingMissileNum[8].size = { 70,70 };

	remainingMissileNum[8].spriteUpdata(true);
}

void Player::addMissile(Enemy* enemy, int& targetnum)
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	if (targetnum <= 0)
	{
		isShotMissile = false;
		waitMissileTime = nextMissileTime;
		return;
	}

	if (!enemy->isTargetSet || enemy->isSetMissile)
	{
		return;
	}

	waitMissileTime--;

	if (waitMissileTime <= 0)
	{
		//�~�T�C���ǉ�
		std::unique_ptr<Missile> newMissile = std::make_unique<Missile>();
		newMissile->init();
		newMissile->setPenemy(enemy);
		newMissile->start(playerObject->getPosition());
		missilesList.push_back(std::move(newMissile));

		//���˃p�[�e�B�N���ǉ�
		SingleParticle newparticle;
		newparticle.generate();
		newparticle.set(5, playerObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 6.0, 6.0);
		newparticle.color = { 1,1,1,0.5f };
		newparticle.isAddBlend = true;
		particleManagerOnTime::addParticle(newparticle, "effect1.png");

		enemy->isSetMissile = true;

		targetnum--;

		waitMissileTime = nextMissileTime;

		//�~�T�C���������������J�E���g(�`���[�g���A��)
		missileCount++;
	}
}

//���Z�b�g
void Player::reset()
{
	isAlive = true;
	isStagingSet = false;
	playerHP = maxHP;
	isBoost = false;
	boostGauge = maxBoostGauge;
	moveSpeed = defaultMoveSpeed;
	armorTime = 0;
	isArmor = false;
	isInvisible = -1;
	isDangerHP = false;
	cautionHPCount = 0;
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
	object3dFBX::SetCamera(followCamera);
}

//�`��
void Player::draw3D(directX* directx)
{
	//�v���C���[�{�̂̕`��
	if (isInvisible == -1)
	{
		playerObject->Draw(directx->cmdList.Get());
	}

	if (isStop)
	{
		return;
	}

	if (!isAlive)
	{
		return;
	}

	//�ʏ�e�̕`��(���j�[�N���X�g)
	for (std::unique_ptr<bullet>& bullet : bulletsList)
	{
		bullet->draw(directx);
	}
	//�~�T�C���̕`��(���j�[�N���X�g)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->draw(directx);
	}
}

void Player::draw2D(directX* directx, int targetnum)
{
	if (!isAlive)
	{
		return;
	}

	if (isStop)
	{
		return;
	}

	targetThird.drawSprite(directx->cmdList.Get());
	targetSecond.drawSprite(directx->cmdList.Get());
	targetFirst.drawSprite(directx->cmdList.Get());

	if (isArmor && armorTime < 5)
	{
		damage.drawSprite(directx->cmdList.Get());
	}

	if (targetCount > 5 && targetCount <= 70)
	{
		rockonGauge[0].drawSprite(directx->cmdList.Get());
		rockonGauge[1].drawSprite(directx->cmdList.Get());
	}

	if (isRockOn)
	{
		for (int i = 0; i < MaxPlayerMissileNum - targetnum; i++)
		{
			remainingMissileNum[i].drawSprite(directx->cmdList.Get());
		}

		if (targetnum >= MaxPlayerMissileNum)
		{
			remainingMissileNum[8].drawSprite(directx->cmdList.Get());
		}
	}

	gaugeFrame.drawSprite(directx->cmdList.Get());
	boostGaugeBar.drawSprite(directx->cmdList.Get());
	if (isDangerHP)
	{
		dangarHPGaugeBar.drawSprite(directx->cmdList.Get());
	}
	HPGaugeBar.drawSprite(directx->cmdList.Get());

	if (isCautionDraw)
	{
		outAreaCaution.drawSprite(directx->cmdList.Get());
	}
}

void Player::drawMiniMapIcon(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	if (isStop)
	{
		return;
	}

	miniMapPlayer.drawSprite(directx->cmdList.Get());
}
