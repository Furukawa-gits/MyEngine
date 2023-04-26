#include"GameScene.h"
#include"../FbxLoder/Object3d_FBX.h"
#include<cassert>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	enemyList.clear();

	Enemy::staticDestroy();
	enemyBullet::staticDestroy();

	directx.release();
	input.release();
	audio.release();
}

//�T�E���h�����ǂݍ��݊֐�
void GameScene::loadSounds()
{

}

//�X�v���C�g(�V�[�����œ���������)������
void GameScene::loadSprites()
{
	//�w�i
	sample_back = std::make_unique<SingleSprite>();
	sample_back->size = { 1280,720 };
	sample_back->generateSprite("sample_back.jpg");

	//�^�C�g��
	titleIconDrag.anchorpoint = { 0.5f,0.5f };
	titleIconDrag.size = { 250,110 };
	titleIconDrag.position = { 0,200,0 };
	titleIconDrag.generateSprite("titleDrag.png");

	titleIconShoot.anchorpoint = { 0.5f,0.5f };
	titleIconShoot.size = { 300,100 };
	titleIconShoot.position = { 0,310,0 };
	titleIconShoot.generateSprite("titleShoot.png");

	titlePlayer.anchorpoint = { 0.5f,0.5f };
	titlePlayer.size = { 410,310 };
	titlePlayer.position = { 600,300,0 };
	titlePlayer.generateSprite("titlePlayer.png");

	titleWhiteBack.size = { 1280,720 };
	titleWhiteBack.generateSprite("white1x1.png");

	//�X�^�[�g�{�^��
	startButton.anchorpoint = { 0.5f,0.5f };
	startButton.size = { 430,50 };
	startButton.position = { 640,500,0 };
	startButton.generateSprite("startButton.png");

#pragma region //�X�e�[�W�A�C�R��
	stage[0].anchorpoint = { 0.5f,0.5f };
	stage[0].size = { 500,100 };
	stage[0].position = { 0,360,0 };
	stage[0].generateSprite("titleText.png");

	stage[1].anchorpoint = { 0.5f,0.5f };
	stage[1].size = { 128,128 };
	stage[1].position = { 0,360,0 };
	stage[1].generateSprite("count0.png");

	stage[2].anchorpoint = { 0.5f,0.5f };
	stage[2].size = { 128,128 };
	stage[2].position = { 0,360,0 };
	stage[2].generateSprite("count1.png");

	stage[3].anchorpoint = { 0.5f,0.5f };
	stage[3].size = { 128,128 };
	stage[3].position = { 0,360,0 };
	stage[3].generateSprite("count2.png");

	stage[4].anchorpoint = { 0.5f,0.5f };
	stage[4].size = { 128,128 };
	stage[4].position = { 0,360,0 };
	stage[4].generateSprite("count3.png");
#pragma endregion //�X�e�[�W�A�C�R��

	//�Z���N�g��ʖ��
	selects[0].anchorpoint = { 0.5f,0.5f };
	selects[0].size = { 100,100 };
	selects[0].position = { 640 - 400,380,0 };
	selects[0].generateSprite("selectL.png");

	selects[1].anchorpoint = { 0.5f,0.5f };
	selects[1].size = { 100,100 };
	selects[1].position = { 640 + 400,380,0 };
	selects[1].generateSprite("selectR.png");

	//�v���C�X�^�[�g�e�L�X�g
	spaceStart.anchorpoint = { 0.5f,0.5f };
	spaceStart.size = { 350,50 };
	spaceStart.position = { 640,530,0 };
	spaceStart.generateSprite("spacestart.png");

	//����{�^��
	selects[2].anchorpoint = { 0.5f,0.5f };
	selects[2].size = { 150,5 };
	selects[2].position = { 640,420,0 };
	selects[2].generateSprite("selectIcon.png");

	toTutorial.anchorpoint = { 0.5f,0.5f };
	toTutorial.size = { 220,50 };
	toTutorial.position = { 640,470,0 };
	toTutorial.generateSprite("toTutorial.png");

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

	//���U���g���
	resultScreen[0].size = { 1280,720 };
	resultScreen[0].generateSprite("black_color.png");

	resultScreen[1].size = { 1280,300 };
	resultScreen[1].generateSprite("black_color.png");

	//�N���A�E�I�[�o�[���
	clearText.anchorpoint = { 0.5f,0.0f };
	clearText.size = { 640,100 };
	clearText.generateSprite("CLEAR_text.png");

	overText.anchorpoint = { 0.5f,0.0f };
	overText.size = { 640,100 };
	overText.generateSprite("OVER_text.png");

	//�^�C�g���{�^��
	titleButton.anchorpoint = { 0.5f,0.5f };
	titleButton.size = { 200,40 };
	titleButton.position = { 640 + 150,500,0 };
	titleButton.generateSprite("titleText.png");

	//�Z���N�g�{�^��
	selectButton.anchorpoint = { 0.5f,0.5f };
	selectButton.size = { 200,40 };
	selectButton.position = { 640 - 150,500,0 };
	selectButton.generateSprite("selectText.png");

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

#pragma region //�E�F�[�u�̕\��
	enemyWaveBar.anchorpoint = { 0.5f,0.0f };
	enemyWaveBar.size = { 10,totalWaveBarLength };
	enemyWaveBar.position = { waveBarPosX,360 - totalWaveBarLength / 2,0 };
	enemyWaveBar.generateSprite("enemyWaveBar.png");

	playerWaveIcon.anchorpoint = { 0.5f,0.5f };
	playerWaveIcon.size = { 120,75 };
	playerWaveIcon.position = { waveBarPosX + 20,360 - totalWaveBarLength / 2,0 };
	playerWaveIcon.generateSprite("playerWaveIcon.png");
#pragma endregion //�E�F�[�u�̕\��

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

	//�}�E�X�J�[�\��
	mouseCursur.anchorpoint = { 0.5f,0.5f };
	mouseCursur.size = { 20,20 };
	mouseCursur.generateSprite("Target.png");

	mouseCursurSub.anchorpoint = { 0.5f,0.5f };
	mouseCursurSub.size = { 20,20 };
	mouseCursurSub.generateSprite("Target.png");
}

//������
void GameScene::init(directX* directx, dxinput* input, Audio* audio)
{
	//null�`�F�b�N
	assert(directx);
	assert(input);
	assert(audio);

	//������������N
	this->directx = std::make_unique<directX>();
	this->directx.reset(directx);
	this->input = std::make_unique<dxinput>();
	this->input.reset(input);
	this->audio = std::make_unique<Audio>();
	this->audio.reset(audio);

	//���ǂݍ���
	loadSounds();

	//�X�v���C�g�N���X������
	SingleSprite::setStaticData(directx->dev.Get(), input);

	SingleParticle::particleStaticInit(directx, nullptr);

	//�f�o�b�O�e�L�X�g������
	debugtext.Init();

	//�X�v���C�g����
	loadSprites();

	//���C�g����
	Light::Staticinitialize(directx->dev.Get());
	light = Light::Create();
	light->SetLightColor({ 1,1,1 });
	light->SetLightDir({ 0,-1,0,0 });

	//3d�I�u�W�F�N�g����
	object3dFBX::setLight(light);

	object3dFBX::SetDevice(directx->dev.Get());

	object3dFBX::CreateGraphicsPipeline();

	object3dFBX::CreateGraphicsPipelineSimple();

	Model* sky = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");
	Model* ground = FbxLoader::GetInstance()->LoadmodelFromFile("floar");

	SkyModel = std::make_unique<Model>();
	SkyModel.reset(sky);
	groundModel = std::make_unique<Model>();
	groundModel.reset(ground);

	//�v���C���[������
	playerPointer = std::make_unique<Player>();
	playerPointer->init(input, directx);

	skySphere = std::make_unique<object3dFBX>();
	skySphere->initialize();
	skySphere->SetModel(SkyModel.get());
	skySphere->SetScale({ 8.0f,8.0f,8.0f });

	groundPlane = std::make_unique<object3dFBX>();
	groundPlane->initialize();
	groundPlane->SetModel(groundModel.get());
	groundPlane->SetPosition(playerPointer->groundPosition);
	groundPlane->SetScale({ 0.5f,0.5f,0.5f });

	srand(time(NULL));

	//�G���f���̏�����
	Enemy::staticInit();

	//�p�[�e�B�N���̋��ʃJ������ݒ�
	SingleParticle::setCamera(playerPointer->followCamera);

	//�{�X�̏�����
	Boss::staticInitBoss();
	testBoss = std::make_unique<Boss>();
	testBoss->bossInit();

	//�{�X(���j�b�g)
	uniteBoss::uniteBossStaticInit(playerPointer.get());
	testUniteBoss = std::make_unique<uniteBoss>();
	testUniteBoss->uniteBossInit();

	//�}�E�X�J�[�\����\��
	ShowCursor(false);

	//�^�C�g���A�j���[�V��������
	titleDragEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, dragEaseStart, dragEaseEnd);
	titleShootEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, shootEaseStart, shootEaseEnd);
	isTitleAnimation = true;
	titleWhiteBackAlpha = 1.0f;
}

//�f�o�b�O�e�L�X�g
void GameScene::debugsPrint()
{
	debugtext.Print("MouseDrag : Camera", 10, 55, 1.0f);
	debugtext.Print("MouseClick : Shot", 10, 70, 1.0f);
	debugtext.Print("MousePress(Left)&Drag : Target", 10, 85, 1.0f);
	debugtext.Print("MouseRelease : Homing", 10, 100, 1.0f);
	//debugtext.Print("R : Reset", 10, 130, 1.0f);

	if (scene == sceneType::title)
	{
		debugtext.Print("Title", 10, 160, 1.0f);
		debugtext.Print("SPACE : start", 10, 190, 1.0f);
	}
	else if (scene == sceneType::select)
	{
		debugtext.Print("Select", 10, 160, 1.0f);
		debugtext.Print("LEFT RIGHT : stage select", 10, 180, 1.0f);
	}
	else if (scene == sceneType::play)
	{
		debugtext.Print("Play", 10, 160, 1.0f);
	}
	else if (scene == sceneType::clear)
	{
		debugtext.Print("clear", 10, 160, 1.0f);
	}
	else
	{
		debugtext.Print("over", 10, 160, 1.0f);
	}
}

#pragma region �e�V�[���X�V

//�^�C�g����ʍX�V
void GameScene::titleUpdata()
{
	if (scene != sceneType::title)
	{
		return;
	}

	titleIconDrag.spriteUpdata();
	titleIconShoot.spriteUpdata();

	resultScreen[0].spriteUpdata();

	if (!titleDragEase.getIsActive() && !titleShootEase.getIsActive() && isTitleAnimation)
	{
		isTitleAnimation = false;
	}

	if (isTitleAnimation)
	{
		titleIconDrag.position.x = titleDragEase.easing();
		titleIconShoot.position.x = titleShootEase.easing();

		return;
	}
	else
	{
		titleIconDrag.position.x = dragEaseEnd;
		titleIconShoot.position.x = shootEaseEnd;
		titleWhiteBackAlpha -= 1.0f / 240.0f;
		titleWhiteBack.color = { 1,1,1,titleWhiteBackAlpha };
	}

	titleWhiteBack.spriteUpdata();
	titlePlayer.spriteUpdata();

	if ((startButton.isSpriteMouseInput() || input->Triger(DIK_SPACE)) && !isPushStart && !isTitleAnimation)
	{
		startButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, startButton.size.y, 0);
		startButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, startButton.size.x, startButton.size.x + 30);
		titleWhiteBackAlpha = 0.0f;
		isPushStart = true;
	}

	if (isPushStart)
	{
		startButton.size.y = startButtonEase_y.easing();
		startButton.size.x = startButtonEase_x.easing();
	}

	int test = sizeof(enemyPattern);

	startButton.spriteUpdata();
	resultScreen[0].spriteUpdata();

	if (isPushStart && !startButtonEase_y.getIsActive())
	{
		stage[0].position.x = 640 - 300;
		isMoveStageIcon = false;
		isPushStart = false;
		stageNum = 0;
		selectIconSizeX = 350;
		scene = sceneType::select;
	}
}

//�Z���N�g��ʍX�V
void GameScene::selectUpdata()
{
	if (scene != sceneType::select)
	{
		return;
	}

	//�X�e�[�W�A�C�R���������Ă��Ȃ���΃Z���N�g�{�^����t
	if (!isMoveStageIcon)
	{
		float iconPos = stage[0].position.x;

		//���̃X�e�[�W
		if (selects[1].isSpriteMouseInput() && stageNum < maxStageNum)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Cubic, 20, iconPos, iconPos - 300);
			stageNum++;
			isMoveStageIcon = true;
		}
		//�O�̃X�e�[�W
		else if (selects[0].isSpriteMouseInput() && stageNum > -1)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Quadratic, 20, iconPos, iconPos + 300);
			stageNum--;
			isMoveStageIcon = true;
		}
	}
	else
	{
		stage[0].position.x = stageIconEase.easing();

		if (!stageIconEase.getIsActive())
		{
			isMoveStageIcon = false;
		}
	}

	stage[1].position.x = stage[0].position.x + 300;
	stage[2].position.x = stage[1].position.x + 300;
	stage[3].position.x = stage[2].position.x + 300;
	stage[4].position.x = stage[3].position.x + 300;

	//�Z���N�g�A�C�R���ʏ�g�k
	if (!isPushStart)
	{
		selectIconSizeX -= 0.5f;

		if (selectIconSizeX <= 300)
		{
			selectIconSizeX = 350;
		}
		selects[2].size = { selectIconSizeX,10 };
	}

	//�X�e�[�W�I�����[�u���łȂ���Ό���
	if (input->Triger(DIK_SPACE) && !isMoveStageIcon && !isPushStart)
	{
		targetnum = 0;

		startButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 20, 0);
		startButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 300, 350);
		isPushStart = true;
	}

	//�X�^�[�g�{�^���g�k
	if (isPushStart)
	{
		selects[2].size.y = startButtonEase_y.easing();
		selects[2].size.x = startButtonEase_x.easing();
	}

	//�e�X�v���C�g�X�V
	for (int i = 0; i < 5; i++)
	{
		stage[i].spriteUpdata();
	}

	for (int i = 0; i < 3; i++)
	{
		selects[i].spriteUpdata();
	}

	spaceStart.spriteUpdata();

	toTutorial.spriteUpdata();

	if (isPushStart && !startButtonEase_y.getIsActive())
	{
		isPushStart = false;

		//�^�C�g���ɖ߂�
		if (stageNum == -1)
		{
			startButton.size = { 340,50 };

			//�^�C�g���A�j���[�V��������
			titleDragEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, dragEaseStart, dragEaseEnd);
			titleShootEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, shootEaseStart, shootEaseEnd);
			isTitleAnimation = true;
			titleWhiteBackAlpha = 1.0f;
			titleIconDrag.position.x = dragEaseStart;
			titleIconShoot.position.x = shootEaseStart;
			titleIconDrag.spriteUpdata();
			titleIconShoot.spriteUpdata();
			scene = sceneType::title;
			return;
		}
		//�`���[�g���A���J�n
		else if (stageNum == 0)
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

			isMoveScreen = false;
			isScreenEase = false;
			isTextEase = false;
			isPushTitle = false;
			nowStageLevel = 1;
			playerPointer->updata();
			object3dFBX::SetCamera(playerPointer->followCamera);

			isTutorial = true;
			scene = sceneType::play;
			return;
		}
		else
		{
			//�X�e�[�W�ǂݍ���
			isLoadStage = loadStage();
		}

		if (isLoadStage)
		{
			//�X�^�[�g�̃J�E���g�_�E����ݒ�
			countDownEase.set(easingType::easeOut, easingPattern::Quintic, countDownTime, 450, 0);
			countDownSprite[0].rotation = 0;
			countDownSprite[1].rotation = 0;
			countDownSprite[2].rotation = 0;
			countDownNum = 0;
			isCountDown = true;
			isStartIcon = false;

			isMoveScreen = false;
			isScreenEase = false;
			isTextEase = false;
			isPushTitle = false;

			nowStageLevel = 1;

			playerPointer->updata();
			object3dFBX::SetCamera(playerPointer->followCamera);

			scene = sceneType::play;
		}
	}
}

//�v���C��ʍX�V
void GameScene::playUpdata()
{
	if (scene != sceneType::play)
	{
		return;
	}

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

		if (testBoss->isTargetSet && !testBoss->isSetMissile)
		{
			playerPointer->addMissile(testBoss.get());

			testBoss->isSetMissile = true;
		}

		if (testUniteBoss->isTargetSet && !testUniteBoss->isSetMissile)
		{
			playerPointer->addMissile(testUniteBoss.get());

			testUniteBoss->isSetMissile = true;
		}

		for (int i = 0; i < targetnum; i++)
		{
			for (std::unique_ptr<uniteParts>& newparts : testUniteBoss->partsList)
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
	if (enemyList.size() <= 0 && nowStageLevel < stageLevel)
	{
		nowStageLevel++;

		playerWaveIcon.position.y += nextWaveDis;

		//�Ō�̃E�F�[�u�Ȃ�{�X��
		if (nowStageLevel == stageLevel && !isBoss)
		{
			if (stageNum < 3)
			{
				//�{�X�o��
				testBoss->bossSet({ 0,5,0 });
				isBoss = true;
			}
			else
			{
				//���j�b�g�{�X�o��
				testUniteBoss->uniteBossSet();
				isBoss = true;
			}
		}
		//�łȂ���Ύ��̓G�R
		else
		{
			//���E�F�[�u
			enemySpawnNum = (rand() % 4) + 3 + stageNum;	//�G�o����
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
			testBoss->bossUpdate(playerPointer.get());
		}
		else
		{
			//���j�b�g�{�X�X�V
			testUniteBoss->uniteBossUpdata();
		}
	}

	//�E�F�[�uUI�̍X�V
	enemyWaveBar.spriteUpdata();

	for (std::unique_ptr<SingleSprite>& newsprite : enemyWaveIcons)
	{
		newsprite->spriteUpdata();
	}

	playerWaveIcon.spriteUpdata();

	//�{�X��|����or�v���C���[�����񂾂烊�U���g
	if (isBoss && (!testBoss->isDraw) && (!testUniteBoss->isDraw))
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.spriteUpdata();
		selects[2].position = { 640 - 150,540,0 };
		selectIconSizeX = 250;
		isSelectOrTitle = -1;
		ButtonEase_y.reSet();
		ButtonEase_x.reSet();

		//�X�e�[�W�N���A
		scene = sceneType::clear;
	}

	if (!playerPointer->isAlive && !playerPointer->isOverStaging && playerPointer->playerHP <= 0)
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.spriteUpdata();
		selects[2].position = { 640 - 150,540,0 };
		selectIconSizeX = 250;
		isSelectOrTitle = -1;
		ButtonEase_y.reSet();
		ButtonEase_x.reSet();

		//�Q�[���I�[�o�[
		scene = sceneType::over;
	}
}

//���U���g��ʍX�V
void GameScene::resultUpdata()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	//��ʔw�i�̃C�[�W���O
	if (isScreenEase)
	{
		resultScreen[0].position.y = resultScreenEase.easing();
		resultScreen[1].position.y = resultScreen[0].position.y;

		clearText.position.x = 640;
		clearText.position.y = resultScreen[0].position.y;
		clearText.spriteUpdata();

		overText.position.x = 640;
		overText.position.y = resultScreen[0].position.y;
		overText.spriteUpdata();

		if (!resultScreenEase.getIsActive())
		{
			isScreenEase = false;

			if (scene == sceneType::clear)
			{
				clearTextEase.set(easingType::easeOut, easingPattern::Quadratic, 25, 200, 300);
			}
			else
			{
				overTextEase.set(easingType::easeOut, easingPattern::Quadratic, 25, 200, 300);
			}
			isTextEase = true;
		}
	}
	else
	{
		resultScreen[0].position.y = 0;
		resultScreen[1].position.y = 0;
	}

	resultScreen[0].spriteUpdata();
	resultScreen[1].spriteUpdata();

	//CLEAR�EOVER�̃e�L�X�g�̃C�[�W���O
	if (isTextEase)
	{
		clearText.position.x = 640;
		clearText.position.y = clearTextEase.easing();
		clearText.spriteUpdata();

		overText.position.x = 640;
		overText.position.y = overTextEase.easing();
		overText.spriteUpdata();

		if (!clearTextEase.getIsActive() && !overTextEase.getIsActive())
		{
			isTextEase = false;
		}
	}

	if (!clearTextEase.getIsActive() && !overTextEase.getIsActive() && !resultScreenEase.getIsActive())
	{
		isMoveScreen = false;
	}
	//�w�i�ƃe�L�X�g�𓮂����I����Ă��Ȃ���΃{�^���̏����͖���
	if (isMoveScreen)
	{
		return;
	}

	bool isTitleButton = titleButton.isSpriteMouseInput();
	bool isSelectButton = selectButton.isSpriteMouseInput();

	titleButton.spriteUpdata();
	selectButton.spriteUpdata();

	if (!isMoveSelectIcon)
	{
		//select���
		if (titleButton.isMouseSelect && isSelectOrTitle == -1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 - 150, 640 + 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
		//title���
		else if (selectButton.isMouseSelect && isSelectOrTitle == 1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 + 150, 640 - 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
	}
	else
	{
		selects[2].position = { selectEase.easing(),540,0 };

		if (!selectEase.getIsActive())
		{
			isMoveSelectIcon = false;
		}
	}

	//�Z���N�g�A�C�R���ʏ�g�k
	if (!isPushTitle && !isMoveSelectIcon)
	{
		selectIconSizeX -= 0.5f;

		if (selectIconSizeX <= 250)
		{
			selectIconSizeX = 200;
		}
		selects[2].size = { selectIconSizeX,7 };
	}

	selects[2].spriteUpdata();

	if ((input->Triger(DIK_SPACE) || isTitleButton || isSelectButton) && !isPushTitle && !isMoveSelectIcon)
	{
		ButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 7, 0);
		ButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 200, 300);
		isPushTitle = true;
	}

	if (isPushTitle)
	{
		selects[2].size.y = ButtonEase_y.easing();
		selects[2].size.x = ButtonEase_x.easing();
	}

	//�^�C�g����ʏ���
	if (isPushTitle && !ButtonEase_y.getIsActive())
	{
		if (isSelectOrTitle == -1)
		{
			isMoveStageIcon = false;
			isPushStart = false;
			selectIconSizeX = 350;
			selects[2].position = { 640,420,0 };
			if (stageNum < 3)
			{
				stageNum++;
				stage[0].position.x -= 300;
			}
			scene = sceneType::select;
		}
		else
		{
			startButton.size = { 340,50 };
			startButton.spriteUpdata();
			isPushStart = false;
			selects[2].position = { 640,420,0 };
			stageNum = 0;
			stage[0].position.x = 640 - 300;

			//�^�C�g���A�j���[�V��������
			titleDragEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, dragEaseStart, dragEaseEnd);
			titleShootEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, shootEaseStart, shootEaseEnd);
			isTitleAnimation = true;
			titleWhiteBackAlpha = 1.0f;
			titleIconDrag.position.x = dragEaseStart;
			titleIconShoot.position.x = shootEaseStart;
			titleIconDrag.spriteUpdata();
			titleIconShoot.spriteUpdata();

			scene = sceneType::title;
		}
	}
}

#pragma endregion �e�V�[���X�V

#pragma region �e�V�[���`��

//�^�C�g����ʕ`��
void GameScene::titleDraw3d()
{
	if (scene != sceneType::title)
	{
		return;
	}
}
void GameScene::titleDraw2d()
{
	if (scene != sceneType::title)
	{
		return;
	}

	resultScreen[0].drawSprite(directx->cmdList.Get());

	if (!isTitleAnimation)
	{
		titleWhiteBack.drawSprite(directx->cmdList.Get());
		titlePlayer.drawSprite(directx->cmdList.Get());
		startButton.drawSprite(directx->cmdList.Get());
	}

	titleIconDrag.drawSprite(directx->cmdList.Get());
	titleIconShoot.drawSprite(directx->cmdList.Get());

	mouseCursurSub.drawSprite(directx->cmdList.Get());
	mouseCursur.drawSprite(directx->cmdList.Get());
}

//�Z���N�g��ʕ`��
void GameScene::selectDraw3d()
{
	if (scene != sceneType::select)
	{
		return;
	}
}
void GameScene::selectDraw2d()
{
	if (scene != sceneType::select)
	{
		return;
	}

	resultScreen[0].drawSprite(directx->cmdList.Get());
	stage[0].drawSprite(directx->cmdList.Get());
	stage[1].drawSprite(directx->cmdList.Get());
	stage[2].drawSprite(directx->cmdList.Get());
	stage[3].drawSprite(directx->cmdList.Get());
	stage[4].drawSprite(directx->cmdList.Get());

	selects[0].drawSprite(directx->cmdList.Get());
	selects[1].drawSprite(directx->cmdList.Get());
	selects[2].drawSprite(directx->cmdList.Get());

	if (!isTutorial && stageNum > 0)
	{
		toTutorial.drawSprite(directx->cmdList.Get());
	}

	spaceStart.drawSprite(directx->cmdList.Get());

	mouseCursurSub.drawSprite(directx->cmdList.Get());
	mouseCursur.drawSprite(directx->cmdList.Get());
}

//�v���C��ʕ`��
void GameScene::playDraw3d()
{
	if (scene != sceneType::play)
	{
		return;
	}

	groundPlane->Draw(directx->cmdList.Get());
	skySphere->Draw(directx->cmdList.Get());

	//�v���C���[�`��
	playerPointer->draw3D(directx.get());

	//�G
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw3D(directx.get());
	}

	//�{�X�`��
	testBoss->bossDraw3D(directx.get());

	testUniteBoss->uniteBossDraw3d(directx.get());
}
void GameScene::playDraw2d()
{
	if (scene != sceneType::play)
	{
		return;
	}

	if (stageNum > 0)
	{
		playerPointer->draw2D(directx.get(), targetnum);
	}

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw2D(directx.get());
	}

	//�{�X�`��(2d)
	testBoss->bossDraw2D(directx.get());

	testUniteBoss->uniteBossDraw2d(directx.get());

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

		playerPointer->draw2D(directx.get(), targetnum);
	}
}

//���U���g��ʕ`��
void GameScene::resultDraw3d()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	groundPlane->Draw(directx->cmdList.Get());
	skySphere->Draw(directx->cmdList.Get());

	//�v���C���[�`��
	playerPointer->draw3D(directx.get());

	//�{�X�`��
	testBoss->bossDraw3D(directx.get());
}
void GameScene::resultDraw2d()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	resultScreen[0].drawSprite(directx->cmdList.Get());
	if (scene == sceneType::clear)
	{
		clearText.drawSprite(directx->cmdList.Get());
	}
	else
	{
		overText.drawSprite(directx->cmdList.Get());
	}
	resultScreen[1].drawSprite(directx->cmdList.Get());

	if (isMoveScreen)
	{
		return;
	}
	titleButton.drawSprite(directx->cmdList.Get());
	selectButton.drawSprite(directx->cmdList.Get());
	selects[2].drawSprite(directx->cmdList.Get());

	mouseCursurSub.drawSprite(directx->cmdList.Get());
	mouseCursur.drawSprite(directx->cmdList.Get());
}

#pragma endregion �e�V�[���`��

//�X�V
void GameScene::updata()
{
	//�}�E�X���W�X�V
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y,0.0f };

	//�}�E�X�J�[�\���X�V
	mouseCursur.position = MOUSE_POS;
	mouseCursur.rotation += 2.0f;
	mouseCursur.spriteUpdata();
	mouseCursurSub.position = MOUSE_POS;
	mouseCursurSub.rotation -= 4.0f;
	mouseCursurSub.spriteUpdata();

	//���C�g�X�V
	light->Update();

	//�V�[���؂�ւ�

	//�^�C�g�����
	titleUpdata();

	//�Z���N�g���
	selectUpdata();

	//�v���C���
	playUpdata();

	//�N���A���
	if (scene == sceneType::clear || scene == sceneType::over)
	{
		resultUpdata();
	}

	sample_back->spriteUpdata();

	debugsPrint();
}

//�w�i�X�v���C�g�`��
void GameScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

//�`��
void GameScene::draw3D()
{
	//�Q�[�����V�[�����Ƃ̕`��
	titleDraw3d();

	selectDraw3d();

	playDraw3d();

	if (scene == sceneType::clear || scene == sceneType::over)
	{
		resultDraw3d();
	}
}
void GameScene::draw2D()
{
	titleDraw2d();

	selectDraw2d();

	playDraw2d();

	resultDraw2d();

	//debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::drawNowWave()
{
	if (stageNum <= 0)
	{
		return;
	}

	if (playerPointer->isStop)
	{
		return;
	}

	enemyWaveBar.drawSprite(directx->cmdList.Get());

	for (std::unique_ptr<SingleSprite>& newsprite : enemyWaveIcons)
	{
		newsprite->drawSprite(directx->cmdList.Get());
	}

	playerWaveIcon.drawSprite(directx->cmdList.Get());

}

void GameScene::drawPositionUI()
{
	if (playerPointer->isStop)
	{
		return;
	}

	//���x���[�^�[
	heightGauge.drawSprite(directx->cmdList.Get());

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->drawMiniMapIcon(directx.get());
	}

	testBoss->drawMiniMapIcon(directx.get());

	testUniteBoss->drawMiniMapIcon(directx.get());

	playerHeight.drawSprite(directx->cmdList.Get());
	playerHeightIcon.drawSprite(directx->cmdList.Get());
}

void GameScene::checkHitPlayerTarget()
{
	//�ʏ�̓G
	checkHitManager::checkRockonEnemys(playerPointer.get(), &enemyList, targetnum);

	//�{�X
	checkHitManager::checkRockonEnemy(playerPointer.get(), testBoss.get(), targetnum);

	checkHitManager::checkRockonEnemy(playerPointer.get(), testUniteBoss.get(), targetnum);

	for (std::unique_ptr<uniteParts>& newparts : testUniteBoss->partsList)
	{
		checkHitManager::checkRockonEnemy(playerPointer.get(), newparts.get(), targetnum);
	}
}

void GameScene::countDown()
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

		testBoss->isStop = true;

		testUniteBoss->isStop = true;

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

		testBoss->isStop = false;

		testUniteBoss->isStop = false;
	}

	return;
}

void GameScene::tutorial()
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

		if (testBoss->isTargetSet && !testBoss->isSetMissile)
		{
			playerPointer->addMissile(testBoss.get());

			testBoss->isSetMissile = true;
		}

		targetnum = 0;
	}

	//�G�����ׂē|����or�v���C���[�����񂾂烊�U���g
	if (enemyList.size() <= 0 && !isMoveText && isShootingText)
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		isShootingText = false;
		playerPointer->isBoostTutorial = true;
		playerPointer->isNormalShot = true;
		playerPointer->isHomingMissile = true;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.spriteUpdata();
		selects[2].position = { 640 - 150,540,0 };
		selectIconSizeX = 250;
		isSelectOrTitle = -1;
		ButtonEase_y.reSet();
		ButtonEase_x.reSet();
		scene = sceneType::clear;
	}
	if (!playerPointer->isAlive && !playerPointer->isOverStaging && playerPointer->playerHP <= 0)
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		isShootingText = false;
		playerPointer->isBoostTutorial = true;
		playerPointer->isNormalShot = true;
		playerPointer->isHomingMissile = true;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.spriteUpdata();
		selects[2].position = { 640 - 150,540,0 };
		selectIconSizeX = 250;
		isSelectOrTitle = -1;
		ButtonEase_y.reSet();
		ButtonEase_x.reSet();
		scene = sceneType::over;
	}
}

bool GameScene::loadStage()
{
	if (stageNum < 1)
	{
		return false;
	}

	if (isTutorial == false)
	{
#ifdef _DEBUG
		playerPointer->isBoostTutorial = true;
		playerPointer->isNormalShot = true;
		playerPointer->isHomingMissile = true;
#else
		return false;
#endif

	}

	//�`���[�g���A���̃e�L�X�g��\�����Ȃ��悤�ɂ���
	isMoveText = false;
	isBoostText = false;
	isShotText = false;
	isMissileText = false;
	isShootingText = false;

	for (int i = 0; i < stageNum + 4; i++)
	{
		//�G�@���X�g
		std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
		newenemy->init(enemyPattern::shot);
		newenemy->set({
		(float)(rand() % 50 - 25),
		(float)(rand() % 30 + 15),
		(float)(rand() % 50 - 25) });

		enemyList.push_back(std::move(newenemy));
	}

	//�v���C���[�̃��Z�b�g
	playerPointer->reset();

	isBoss = false;

	if (stageNum == 1)
	{
		stageLevel = stageNum + 2;

		//�G
		testBoss->changePattern(enemyPattern::chase);

		//�G�@���X�g
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::shot);
		}

		//�{�X�̐ݒ�
		testBoss->setHitPoint(stageLevel + 5);
	}
	else if (stageNum == 2)
	{
		stageLevel = stageNum + 2;

		//�G
		testBoss->changePattern(enemyPattern::rampage);

		//�G�@���X�g
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::shot);
		}

		//�{�X�̐ݒ�
		testBoss->setHitPoint(stageLevel + 5);
	}
	else if (stageNum == 3)
	{
		stageLevel = stageNum + 2;

		//�G�@���X�g
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::rampage);
		}
	}

	//�A�C�R�����m�̋������v�Z
	nextWaveDis = totalWaveBarLength / (stageLevel - 1);

	enemyWaveIcons.clear();

	playerWaveIcon.position.y = 360 - totalWaveBarLength / 2;

	for (int i = 0; i < stageLevel - 1; i++)
	{
		std::unique_ptr<SingleSprite> newicon = std::make_unique<SingleSprite>();
		newicon->anchorpoint = { 0.5f,0.5f };
		newicon->size = { 50,50 };
		newicon->position = { waveBarPosX,(360 - totalWaveBarLength / 2) + i * nextWaveDis,0 };
		newicon->generateSprite("enemyWaveIcon.png");
		newicon->spriteUpdata();
		enemyWaveIcons.push_back(std::move(newicon));
	}

	std::unique_ptr<SingleSprite> newicon = std::make_unique<SingleSprite>();
	newicon->anchorpoint = { 0.5f,0.5f };
	newicon->size = { 100,100 };
	newicon->position = { waveBarPosX,(360 - totalWaveBarLength / 2) + totalWaveBarLength,0 };
	newicon->generateSprite("bossWaveIcon.png");
	newicon->spriteUpdata();
	enemyWaveIcons.push_back(std::move(newicon));

	return true;
}

void GameScene::checkHitEverything()
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
	checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, testBoss.get());

	//�ʏ�e�ƃ��j�b�g�{�X�{�̂̓����蔻��
	checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, testUniteBoss.get());

	//�p�[�c�̓����蔻��
	for (std::unique_ptr<uniteParts>& newparts : testUniteBoss->partsList)
	{
		checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, newparts.get());
	}

	//�v���C���[�ƃ{�X�̓����蔻��
	if (!testBoss->getIsAppear() && stageNum < 3)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), testBoss.get());
		checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), testBoss.get());
		checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, testBoss.get());
	}

	//���j�b�g�{�X�{�̂Ƃ̓����蔻��
	if (!testUniteBoss->getIsAppear() && stageNum == 3)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), testUniteBoss.get());

		for (std::unique_ptr<uniteParts>& newparts : testUniteBoss->partsList)
		{
			checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), newparts.get());
			checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, newparts.get());
		}
	}
}