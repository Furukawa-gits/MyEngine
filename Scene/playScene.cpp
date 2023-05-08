#include "playScene.h"

playScene::playScene()
{
	//���\�[�X�ǂݍ���
	loadResources();

	//�p�����[�^�̃Z�b�g
	setParameter();
}

void playScene::loadResources()
{
#pragma region	//�`���[�g���A���p�̃e�L�X�g
	moveText.anchorpoint = { 0.5f,0.5f };
	moveText.size = { 375,75 };
	moveText.position = { 640,600,0 };
	moveText.generateSprite("moveText.png");

	boostText.anchorpoint = { 0.5f,0.5f };
	boostText.size = { 398,75 };
	boostText.position = { 640,600,0 };
	boostText.generateSprite("boostText.png");

	shotText.anchorpoint = { 0.5f,0.5f };
	shotText.size = { 300,75 };
	shotText.position = { 640,600,0 };
	shotText.generateSprite("shotText.png");

	missileText.anchorpoint = { 0.5f,0.5f };
	missileText.size = { 593,94 };
	missileText.position = { 640,600,0 };
	missileText.generateSprite("missileText.png");

	shootingText.anchorpoint = { 0.5f,0.5f };
	shootingText.size = { 398,75 };
	shootingText.position = { 640,600,0 };
	shootingText.generateSprite("shootingText.png");
#pragma endregion //�`���[�g���A���p�̃e�L�X�g

#pragma region	//�J�E���g�_�E���A�C�R��
	countDownSprite[0].anchorpoint = { 0.5f,0.5f };
	countDownSprite[0].size = { 100,100 };
	countDownSprite[0].position = { 640,360,0 };
	countDownSprite[0].generateSprite("count3.png");
	countDownSprite[0].spriteUpdata();

	countDownSprite[1].anchorpoint = { 0.5f,0.5f };
	countDownSprite[1].size = { 100,100 };
	countDownSprite[1].position = { 640,360,0 };
	countDownSprite[1].generateSprite("count2.png");
	countDownSprite[1].spriteUpdata();

	countDownSprite[2].anchorpoint = { 0.5f,0.5f };
	countDownSprite[2].size = { 100,100 };
	countDownSprite[2].position = { 640,360,0 };
	countDownSprite[2].generateSprite("count1.png");
	countDownSprite[2].spriteUpdata();
#pragma endregion //�J�E���g�_�E���A�C�R��

	//�X�^�[�g�e�L�X�g
	playStart.anchorpoint = { 0.5f,0.5f };
	playStart.size = { 500,125 };
	playStart.position = { 640,360,0 };
	playStart.generateSprite("playstart.png");

	//���x���[�^�[
	heightGauge.anchorpoint = { 0.5f,0.5f };
	heightGauge.size = { 40,200 };
	heightGauge.position = { Enemy::miniMapPosition.x + 120,Enemy::miniMapPosition.y,0 };
	heightGauge.generateSprite("playerheightbar.png");

	//�v���C���[�̍��x
	playerHeight.anchorpoint = { 0.5f,0.5f };
	playerHeight.size = { 32,5 };
	playerHeight.generateSprite("playerHPGauge.png");
	playerHeightIcon.anchorpoint = { 0.0f,0.5f };
	playerHeightIcon.size = { 44,17 };
	playerHeightIcon.generateSprite("playerHeightIcon.png");

	//�V���ƒn�ʂ̃��f��
	Model* sky = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");
	Model* ground = FbxLoader::GetInstance()->LoadmodelFromFile("floar");
	SkyModel = std::make_unique<Model>();
	SkyModel.reset(sky);
	groundModel = std::make_unique<Model>();
	groundModel.reset(ground);

	//�V���ƒn�ʂ̃I�u�W�F�N�g������
	skySphere = std::make_unique<object3dFBX>();
	skySphere->initialize();
	skySphere->SetModel(SkyModel.get());
	skySphere->SetScale({ 8.0f,8.0f,8.0f });
	groundPlane = std::make_unique<object3dFBX>();
	groundPlane->initialize();
	groundPlane->SetModel(groundModel.get());
	groundPlane->SetPosition(playerPointer->groundPosition);
	groundPlane->SetScale({ 0.5f,0.5f,0.5f });
}

void playScene::initialize()
{
	loadResources();

	setParameter();
}

void playScene::setParameter()
{
	if (isTutorial)
	{
		//�v���C���[�̃��Z�b�g
		playerPointer->reset();
		playerPointer->cameraMoveCount = 0;
		playerPointer->boostCount = 0;
		playerPointer->normalShotCount = 0;
		playerPointer->missileCount = 0;
		playerPointer->isBoostTutorial = false;
		playerPointer->isNormalShot = false;
		playerPointer->isHomingMissile = false;
		isMoveText = true;
		isBoostText = false;
		isShotText = false;
		isMissileText = false;
		isShootingText = false;
		isBoss = false;
		//�X�^�[�g�̃J�E���g�_�E����ݒ�
		countDownEase.set(easingType::easeOut, easingPattern::Quintic, countDownTime, 450, 0);
		countDownSprite[0].rotation = 0;
		countDownSprite[1].rotation = 0;
		countDownSprite[2].rotation = 0;
		countDownNum = 0;
		isCountDown = true;
		isStartIcon = false;

		nowStageLevel = 1;
		playerPointer->updata();
		object3dFBX::SetCamera(playerPointer->followCamera);
	}
	else
	{
		//�X�^�[�g�̃J�E���g�_�E����ݒ�
		countDownEase.set(easingType::easeOut, easingPattern::Quintic, countDownTime, 450, 0);
		countDownSprite[0].rotation = 0;
		countDownSprite[1].rotation = 0;
		countDownSprite[2].rotation = 0;
		countDownNum = 0;
		isCountDown = true;
		isStartIcon = false;

		nowStageLevel = 1;

		playerPointer->updata();
		object3dFBX::SetCamera(playerPointer->followCamera);
	}
}

void playScene::updata()
{
	if (input->Triger(DIK_SPACE))
	{
		int test = 0;
	}

	//�J�E���g�_�E��
	countDown();

	//�G�ɋ��ʂ��ĕK�v�ȃv���C���[�̏���n��
	Enemy::staticUpdata(playerPointer->getPlayerPos(), playerPointer->getPlayerFront(), playerPointer->isAlive);

	if (!isCountDown && !isStartIcon)
	{
		//�Q�[�����ԃJ�E���g
		totalPlayFlameCount++;
	}

	//�p�[�e�B�N���̋��ʃJ������ݒ�
	SingleParticle::setCamera(playerPointer->followCamera);

	//�X�J�C�h�[���X�V
	skySphere->setRotMatrix(0, 0, skyShpereRotY);
	skySphere->updata();

	//�n�ʍX�V
	groundPlane->updata();

	//���x���[�^�[�X�V
	//�v���C���[��y���W�ɕ␳��������position�ɑ��
	float playerH = playerPointer->getPlayerPos().y - playerPointer->groundPosition.y;

	//�X�v���C�g�̍��W�n�ɕύX�����[�^�[�̏オ���𒲐�
	playerH = -(playerH * 0.3333f);

	//���ۂɕ\��������W�ɕύX
	playerH = playerH + heightGauge.position.y + (heightGauge.size.y / 2);

	float underLimit = heightGauge.position.y + (heightGauge.size.y / 2) - 5;
	float upperLimit = heightGauge.position.y - (heightGauge.size.y / 2) + 5;

	if (playerH < upperLimit)
	{
		playerH = upperLimit;
	}
	else if (playerH > underLimit)
	{
		playerH = underLimit;
	}

	playerHeight.position = { heightGauge.position.x,playerH,0 };
	playerHeightIcon.position = { playerHeight.position.x + 20,playerH - 2,0 };

	heightGauge.spriteUpdata();
	playerHeight.spriteUpdata();
	playerHeightIcon.spriteUpdata();

	//�`���[�g���A��
	if (stageNum == 0)
	{
		tutorial();
		return;
	}

	if (input->Triger(DIK_LSHIFT))
	{
		playerPointer->playerHP = 0;
	}

	//�v���C���[�X�V
	playerPointer->updata();

	//�G�l�~�[�X�V
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->updata();
	}

	//�z�[�~���O�e����
	if (input->Mouse_LeftRelease() && !isCountDown)
	{
		for (int i = 0; i < targetnum; i++)
		{
			for (std::unique_ptr<Enemy>& newenemy : enemyList)
			{
				if (newenemy->isTargetSet && !newenemy->isSetMissile)
				{
					playerPointer->addMissile(newenemy.get());

					newenemy->isSetMissile = true;

					break;
				}
			}
		}

		if (normalBoss->isTargetSet && !normalBoss->isSetMissile)
		{
			playerPointer->addMissile(normalBoss.get());

			normalBoss->isSetMissile = true;
		}

		if (UniteBoss->isTargetSet && !UniteBoss->isSetMissile)
		{
			playerPointer->addMissile(UniteBoss.get());

			UniteBoss->isSetMissile = true;
		}

		for (int i = 0; i < targetnum; i++)
		{
			for (std::unique_ptr<uniteParts>& newparts : UniteBoss->partsList)
			{
				if (newparts->isTargetSet && !newparts->isSetMissile)
				{
					playerPointer->addMissile(newparts.get());

					newparts->isSetMissile = true;

					break;
				}
			}
		}

		targetnum = 0;
	}

	//�����蔻��
	checkHitEverything();

	//����ł���G���G���J�E���g
	int count = 0;
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		if (!newenemy->isDraw)
		{
			count++;
		}
	}

	//���ׂĂ̎G���G������ł����玟�̃E�F�[�u
	if (enemyList.size() <= 0 && nowStageLevel < maxStageLevel)
	{
		nowStageLevel++;

		playerWaveIcon->position.y += nextWaveDis;

		//�Ō�̃E�F�[�u�Ȃ�{�X��
		if (nowStageLevel == maxStageLevel && !isBoss)
		{
			if (stageNum < 3)
			{
				//�{�X�o��
				normalBoss->bossSet({ 0,5,0 });
				isBoss = true;
			}
			else
			{
				//���j�b�g�{�X�o��
				UniteBoss->uniteBossSet();
				isBoss = true;
			}
		}
		//�łȂ���Ύ��̓G�R
		else
		{
			//���E�F�[�u
			int enemySpawnNum = (rand() % 4) + 3 + stageNum;	//�G�o����
			int nextType = (rand() % 4) + 1;				//�G�̎��

			for (int i = 0; i < enemySpawnNum; i++)
			{
				//�G�@���X�g
				std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
				newenemy->init(enemyPattern::shot);
				newenemy->set({
				(float)(rand() % 50 - 25),
				(float)(rand() % 30 + 15),
				(float)(rand() % 50 - 25) });

				newenemy->changePattern((enemyPattern)nextType);

				enemyList.push_back(std::move(newenemy));
			}
		}
	}

	if (isBoss)
	{
		if (stageNum < 3)
		{
			//�{�X�X�V
			normalBoss->bossUpdate(playerPointer.get());
		}
		else
		{
			//���j�b�g�{�X�X�V
			UniteBoss->uniteBossUpdata();
		}
	}

	//�E�F�[�uUI�̍X�V
	enemyWaveBar->spriteUpdata();

	for (std::unique_ptr<SingleSprite>& newsprite : enemyWaveIcons)
	{
		newsprite->spriteUpdata();
	}

	playerWaveIcon->spriteUpdata();

	//�{�X��|����or�v���C���[�����񂾂烊�U���g
	if (isBoss && (!normalBoss->isDraw) && (!UniteBoss->isDraw))
	{
		isClearOrOver = true;
		isNextScene = true;
	}

	if (!playerPointer->isAlive && !playerPointer->isOverStaging && playerPointer->playerHP <= 0)
	{
		isClearOrOver = false;
		isNextScene = true;
	}
}

void playScene::draw3D()
{
	groundPlane->Draw(directx->cmdList.Get());
	skySphere->Draw(directx->cmdList.Get());

	//�v���C���[�`��
	playerPointer->draw3D(directx);

	//�G
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw3D(directx);
	}

	//�{�X�`��
	normalBoss->bossDraw3D(directx);

	UniteBoss->uniteBossDraw3d(directx);
}

void playScene::draw2D()
{
	if (stageNum > 0)
	{
		playerPointer->draw2D(directx, targetnum);
	}

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw2D(directx);
	}

	//�{�X�`��(2d)
	normalBoss->bossDraw2D(directx);

	UniteBoss->uniteBossDraw2d(directx);

	if (isCountDown)
	{
		countDownSprite[countDownNum].drawSprite(directx->cmdList.Get());
	}

	if (isStartIcon)
	{
		playStart.drawSprite(directx->cmdList.Get());
	}

	//�E�F�[�u�`��
	drawNowWave();

	//�~�j�}�b�v�����x���[�^�[�`��
	drawPositionUI();

	//�`���[�g���A���e�L�X�g�̓s����A�v���C���[�̕`�揇�͕����Ă���
	if (isTutorial)
	{
		if (isMoveText)
		{
			moveText.drawSprite(directx->cmdList.Get());
		}
		else if (isBoostText)
		{
			boostText.drawSprite(directx->cmdList.Get());
		}
		else if (isShotText && !playerPointer->isStop)
		{
			shotText.drawSprite(directx->cmdList.Get());
		}
		else if (isMissileText)
		{
			missileText.drawSprite(directx->cmdList.Get());
		}
		else if (isShootingText)
		{
			shootingText.drawSprite(directx->cmdList.Get());
		}

		playerPointer->draw2D(directx, targetnum);
	}
}

void playScene::drawNowWave()
{
	if (stageNum <= 0)
	{
		return;
	}

	if (playerPointer->isStop)
	{
		return;
	}

	enemyWaveBar->drawSprite(directx->cmdList.Get());

	for (std::unique_ptr<SingleSprite>& newsprite : enemyWaveIcons)
	{
		newsprite->drawSprite(directx->cmdList.Get());
	}

	playerWaveIcon->drawSprite(directx->cmdList.Get());
}

void playScene::drawPositionUI()
{
	if (playerPointer->isStop)
	{
		return;
	}

	//���x���[�^�[
	heightGauge.drawSprite(directx->cmdList.Get());

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->drawMiniMapIcon(directx);
	}

	normalBoss->drawMiniMapIcon(directx);

	UniteBoss->drawMiniMapIcon(directx);

	playerHeight.drawSprite(directx->cmdList.Get());
	playerHeightIcon.drawSprite(directx->cmdList.Get());
}

void playScene::checkHitPlayerTarget()
{
	//�ʏ�̓G
	checkHitManager::checkRockonEnemys(playerPointer.get(), &enemyList, targetnum);

	//�{�X
	checkHitManager::checkRockonEnemy(playerPointer.get(), normalBoss.get(), targetnum);

	checkHitManager::checkRockonEnemy(playerPointer.get(), UniteBoss.get(), targetnum);

	for (std::unique_ptr<uniteParts>& newparts : UniteBoss->partsList)
	{
		checkHitManager::checkRockonEnemy(playerPointer.get(), newparts.get(), targetnum);
	}
}

void playScene::checkHitEverything()
{
	checkHitManager::checkMissilesEnemy(&playerPointer->missilesList);

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), newenemy.get());
		checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), newenemy.get());
		checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, newenemy.get());
	}

	//�G�����b�N�I������Ă��邩�ǂ���
	checkHitPlayerTarget();

	//�v���C���[�̒ʏ�e�����蔻��
	checkHitManager::checkBulletsEnemys(&playerPointer->bulletsList, &enemyList);

	//�ʏ�e�ƃ{�X�̓����蔻��
	checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, normalBoss.get());

	//�ʏ�e�ƃ��j�b�g�{�X�{�̂̓����蔻��
	checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, UniteBoss.get());

	//�p�[�c�̓����蔻��
	for (std::unique_ptr<uniteParts>& newparts : UniteBoss->partsList)
	{
		checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, newparts.get());
	}

	//�v���C���[�ƃ{�X�̓����蔻��
	if (!normalBoss->getIsAppear() && stageNum < 3)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), normalBoss.get());
		checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), normalBoss.get());
		checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, normalBoss.get());
	}

	//���j�b�g�{�X�{�̂Ƃ̓����蔻��
	if (!UniteBoss->getIsAppear() && stageNum == 3)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), UniteBoss.get());

		for (std::unique_ptr<uniteParts>& newparts : UniteBoss->partsList)
		{
			checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), newparts.get());
			checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, newparts.get());
		}
	}
}

void playScene::countDown()
{
	if (!isCountDown && !isStartIcon)
	{
		return;
	}

	if (isCountDown)
	{
		float countDownSize = countDownEase.easing();
		countDownSprite[countDownNum].size = { countDownSize,countDownSize };
		countDownSprite[countDownNum].rotation -= 4;
		countDownSprite[countDownNum].position = { 640,360,0 };
		countDownSprite[countDownNum].spriteUpdata();

		if (!countDownEase.getIsActive())
		{
			if (countDownNum + 1 < 3)
			{
				countDownEase.set(easingType::easeOut, easingPattern::Quintic, countDownTime, 450, 0);
				countDownNum++;
			}
			else
			{
				isStartIcon = true;
				startIconTime = 40;
				isCountDown = false;
			}
		}
	}

	if (isStartIcon)
	{
		startIconTime--;
		playStart.spriteUpdata();

		if (startIconTime <= 0)
		{
			isStartIcon = false;
		}
	}

	//�X�^�[�g�̃J�E���g�_�E�����o���I������瓮����
	if (isCountDown || isStartIcon)
	{
		playerPointer->isStop = true;

		//�G
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->isStop = true;
		}

		normalBoss->isStop = true;

		UniteBoss->isStop = true;

		return;
	}
	else
	{
		playerPointer->isStop = false;

		//�G
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->isStop = false;
		}

		normalBoss->isStop = false;

		UniteBoss->isStop = false;
	}

	return;
}

void playScene::tutorial()
{
	playerPointer->updata();

	//���ʃJ�����𓮂�������G�o��
	if (playerPointer->cameraMoveCount >= 250 && isMoveText)
	{
		isMoveText = false;
		isBoostText = true;

		playerPointer->isBoostTutorial = true;
	}

	//3��u�[�X�g������G�o��
	if (!playerPointer->isBoost && playerPointer->boostCount > 2 && isBoostText)
	{
		for (int i = 0; i < 20; i++)
		{
			//�G�@���X�g
			std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
			newenemy->init(enemyPattern::tutorial);
			newenemy->set({
			(float)(rand() % 100 - 50),
			(float)(rand() % 60 + 30),
			(float)(rand() % 100 - 50) });

			enemyList.push_back(std::move(newenemy));
		}

		isBoostText = false;
		isShotText = true;

		playerPointer->isNormalShot = true;
	}

	//�G���o�����o�r���Ȃ�J�������Z�b�g����
	int count = 0;
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		if (newenemy->isAppear == true)
		{
			count++;
		}
	}
	if (count > 0)
	{
		playerPointer->isStop = true;
		playerPointer->isInvisible = 1;
		stagingCamera = new Camera;
		stagingCamera->SetEye({ 0,0,-30 });
		stagingCamera->SetTarget({ 0,30,0 });
		stagingCamera->Update();
		object3dFBX::SetCamera(stagingCamera);
	}
	else
	{
		playerPointer->isStop = false;
		playerPointer->isInvisible = -1;
		object3dFBX::SetCamera(playerPointer->followCamera);
	}

	//�ʏ�e -> �~�T�C��
	if (playerPointer->normalShotCount > 3)
	{
		isShotText = false;
		isMissileText = true;

		playerPointer->isHomingMissile = true;
	}

	//�~�T�C�� -> Lets Shooting!!!
	if (playerPointer->missileCount > 1)
	{
		isMissileText = false;
		isShootingText = true;
	}

	XMFLOAT3 playerTargetPos = playerPointer->getTargetPosition();

	if (playerTargetPos.y < 100)
	{
		playerTargetPos.y = playerTargetPos.y + 40;
	}
	else
	{
		playerTargetPos.y = playerTargetPos.y - 40;
	}

	moveText.position = playerTargetPos;

	shotText.position = playerTargetPos;

	moveText.spriteUpdata();
	boostText.spriteUpdata();
	shotText.spriteUpdata();
	missileText.spriteUpdata();
	shootingText.spriteUpdata();

	//�G�l�~�[�X�V
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->updata();
	}

	//�����蔻��
	checkHitEverything();

	//�z�[�~���O�e����
	if (input->Mouse_LeftRelease() && !isCountDown)
	{
		for (int i = 0; i < targetnum; i++)
		{
			for (std::unique_ptr<Enemy>& newenemy : enemyList)
			{
				if (newenemy->isTargetSet && !newenemy->isSetMissile)
				{
					playerPointer->addMissile(newenemy.get());

					newenemy->isSetMissile = true;

					break;
				}
			}
		}

		if (normalBoss->isTargetSet && !normalBoss->isSetMissile)
		{
			playerPointer->addMissile(normalBoss.get());

			normalBoss->isSetMissile = true;
		}

		targetnum = 0;
	}

	//�G�����ׂē|����or�v���C���[�����񂾂烊�U���g
	if (enemyList.size() <= 0 && !isMoveText && isShootingText)
	{
		isClearOrOver = true;
		isNextScene = true;
	}
	if (!playerPointer->isAlive && !playerPointer->isOverStaging && playerPointer->playerHP <= 0)
	{
		isClearOrOver = false;
		isNextScene = true;
	}
}