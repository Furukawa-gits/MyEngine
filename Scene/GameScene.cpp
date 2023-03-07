#include"GameScene.h"
#include"../FbxLoder/Object3d_FBX.h"
#include<cassert>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	delete(object);
	delete(skySphere);
	delete(cameraobj);
	delete(model);
	delete(SkyModel);

	enemyList.clear();

	Enemy::staticDestroy();
	enemyBullet::staticDestroy();
}

//�T�E���h�����ǂݍ��݊֐�
void GameScene::Load_sounds()
{

}

//�X�v���C�g(�V�[�����œ���������)������
void GameScene::Load_Sprites()
{
	//�w�i
	sample_back = std::make_unique<SingleSprite>();
	sample_back->size = { 1280,720 };
	sample_back->GenerateSprite("sample_back.jpg");

	//�^�C�g��
	gameTitle.anchorpoint = { 0.5f,0.5f };
	gameTitle.size = { 500,220 };
	gameTitle.position = { 640,200,0 };
	gameTitle.GenerateSprite("DragShoot.png");

	//�X�^�[�g�{�^��
	startButton.anchorpoint = { 0.5f,0.5f };
	startButton.size = { 340,50 };
	startButton.GenerateSprite("startButton.png");
	startButton.position = { 640,500,0 };
	startButton.SpriteTransferVertexBuffer();

#pragma region //�X�e�[�W�A�C�R���O�`�Q
	stage[0].anchorpoint = { 0.5f,0.5f };
	stage[0].size = { 300,100 };
	stage[0].position = { 0,360,0 };
	stage[0].GenerateSprite("titleText.png");

	stage[1].anchorpoint = { 0.5f,0.5f };
	stage[1].size = { 128,128 };
	stage[1].position = { 0,360,0 };
	stage[1].GenerateSprite("howto.png");

	stage[2].anchorpoint = { 0.5f,0.5f };
	stage[2].size = { 128,128 };
	stage[2].position = { 0,360,0 };
	stage[2].GenerateSprite("count1.png");

	stage[3].anchorpoint = { 0.5f,0.5f };
	stage[3].size = { 128,128 };
	stage[3].position = { 0,360,0 };
	stage[3].GenerateSprite("count2.png");

	stage[4].anchorpoint = { 0.5f,0.5f };
	stage[4].size = { 128,128 };
	stage[4].position = { 0,360,0 };
	stage[4].GenerateSprite("count3.png");
#pragma endregion //�X�e�[�W�A�C�R���O�`�Q

	//�Z���N�g��ʖ��
	selects[0].anchorpoint = { 0.5f,0.5f };
	selects[0].size = { 100,100 };
	selects[0].position = { 640 - 400,360,0 };
	selects[0].GenerateSprite("selectL.png");

	selects[1].anchorpoint = { 0.5f,0.5f };
	selects[1].size = { 100,100 };
	selects[1].position = { 640 + 400,360,0 };
	selects[1].GenerateSprite("selectR.png");

	//����{�^��
	selects[2].anchorpoint = { 0.5f,0.5f };
	selects[2].size = { 150,5 };
	selects[2].position = { 640,420,0 };
	selects[2].GenerateSprite("selectIcon.png");

	toTutorial.anchorpoint = { 0.5f,0.5f };
	toTutorial.size = { 220,50 };
	toTutorial.position = { 640,470,0 };
	toTutorial.GenerateSprite("toTutorial.png");

#pragma region	//�J�E���g�_�E���A�C�R��
	countDownSprite[0].anchorpoint = { 0.5f,0.5f };
	countDownSprite[0].size = { 100,100 };
	countDownSprite[0].position = { 640,360,0 };
	countDownSprite[0].GenerateSprite("count3.png");
	countDownSprite[0].SpriteTransferVertexBuffer();

	countDownSprite[1].anchorpoint = { 0.5f,0.5f };
	countDownSprite[1].size = { 100,100 };
	countDownSprite[1].position = { 640,360,0 };
	countDownSprite[1].GenerateSprite("count2.png");
	countDownSprite[1].SpriteTransferVertexBuffer();

	countDownSprite[2].anchorpoint = { 0.5f,0.5f };
	countDownSprite[2].size = { 100,100 };
	countDownSprite[2].position = { 640,360,0 };
	countDownSprite[2].GenerateSprite("count1.png");
	countDownSprite[2].SpriteTransferVertexBuffer();
#pragma endregion //�J�E���g�_�E���A�C�R��

	playStart.anchorpoint = { 0.5f,0.5f };
	playStart.size = { 500,125 };
	playStart.position = { 640,360,0 };
	playStart.GenerateSprite("playstart.png");

	//���U���g���
	resultScreen[0].size = { 1280,720 };
	resultScreen[0].GenerateSprite("black_color.png");

	resultScreen[1].size = { 1280,300 };
	resultScreen[1].GenerateSprite("black_color.png");

	//�N���A�E�I�[�o�[���
	clearText.anchorpoint = { 0.5f,0.0f };
	clearText.size = { 640,100 };
	clearText.GenerateSprite("CLEAR_text.png");

	overText.anchorpoint = { 0.5f,0.0f };
	overText.size = { 640,100 };
	overText.GenerateSprite("OVER_text.png");

	//�^�C�g���{�^��
	titleButton.anchorpoint = { 0.5f,0.5f };
	titleButton.size = { 150,50 };
	titleButton.GenerateSprite("titleText.png");
	titleButton.position = { 640 + 150,500,0 };
	titleButton.SpriteTransferVertexBuffer();

	//�Z���N�g�{�^��
	selectButton.anchorpoint = { 0.5f,0.5f };
	selectButton.size = { 200,50 };
	selectButton.GenerateSprite("selectText.png");
	selectButton.position = { 640 - 150,500,0 };
	selectButton.SpriteTransferVertexBuffer();

	//�`���[�g���A���p�̃e�L�X�g
	moveText.anchorpoint = { 0.5f,0.5f };
	moveText.size = { 250,50 };
	moveText.position = { 640,600,0 };
	moveText.GenerateSprite("moveText.png");

	shotText.anchorpoint = { 0.5f,0.5f };
	shotText.size = { 200,50 };
	shotText.position = { 640,600,0 };
	shotText.GenerateSprite("shotText.png");

	missileText.anchorpoint = { 0.5f,0.5f };
	missileText.size = { 315,50 };
	missileText.position = { 640,600,0 };
	missileText.GenerateSprite("missileText.png");

	shootingText.anchorpoint = { 0.5f,0.5f };
	shootingText.size = { 265,50 };
	shootingText.position = { 640,600,0 };
	shootingText.GenerateSprite("shootingText.png");
}

//������
void GameScene::Init(directX* directx, dxinput* input, Audio* audio)
{
	//null�`�F�b�N
	assert(directx);
	assert(input);
	assert(audio);

	//������������N
	this->directx = directx;
	this->input = input;
	this->audio = audio;

	//���ǂݍ���
	Load_sounds();

	//�X�v���C�g�N���X������
	SingleSprite::SetStaticData(directx->dev.Get());

	SingleParticle::particleStaticInit(directx, nullptr);

	//�f�o�b�O�e�L�X�g������
	debugtext.Init();

	//�X�v���C�g����
	Load_Sprites();

	//3d�I�u�W�F�N�g����
	Object3d_FBX::SetDevice(directx->dev.Get());

	Object3d_FBX::CreateGraphicsPipeline();

	Object3d_FBX::CreateGraphicsPipelineSimple();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");
	SkyModel = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");

	//�v���C���[������
	player_p = std::make_unique<Player>();
	player_p->init(input, directx);

	skySphere = new Object3d_FBX;
	skySphere->Initialize();
	skySphere->SetModel(SkyModel);
	skySphere->SetScale({ 8.0f,8.0f,8.0f });

	cameraobj = new Object3d_FBX;
	cameraobj->Initialize();
	cameraobj->SetModel(model);

	srand(time(NULL));

	//�G���f���̏�����
	Enemy::staticInit();

	//�p�[�e�B�N���̋��ʃJ������ݒ�
	SingleParticle::setCamera(player_p->followCamera);

	//�{�X�̏�����
	Boss::staticInitBoss();
	testBoss = std::make_unique<Boss>();
	testBoss->bossInit();

	//�{�X(���j�b�g)
	uniteBoss::uniteBossStaticInit(player_p.get());
	testUniteBoss = std::make_unique<uniteBoss>();
	testUniteBoss->uniteBossInit();
}

//�f�o�b�O�e�L�X�g
void GameScene::debugs_print()
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
void GameScene::Title_updata()
{
	if (scene != sceneType::title)
	{
		return;
	}

	if (input->Triger(DIK_SPACE) && !isPushStart)
	{
		startButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 50, 0);
		startButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 340, 420);
		isPushStart = true;
	}

	if (isPushStart)
	{
		startButton.size.y = startButtonEase_y.easing();
		startButton.size.x = startButtonEase_x.easing();
		startButton.SpriteTransferVertexBuffer();
	}

	int test = sizeof(enemyPattern);

	startButton.SpriteUpdate();
	resultScreen[0].SpriteUpdate();
	gameTitle.SpriteUpdate();

	if (isPushStart && !startButtonEase_y.getIsActive())
	{
		stage[0].position.x = 640;
		isMoveStageIcon = false;
		isPushStart = false;
		stageNum = -1;
		selectIconSizeX = 350;
		scene = sceneType::select;
	}
}

//�Z���N�g��ʍX�V
void GameScene::Select_updata()
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
		if (input->Triger(DIK_RIGHT) && stageNum < maxStageNum)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Cubic, 20, iconPos, iconPos - 300);
			stageNum++;
			isMoveStageIcon = true;
		}
		//�O�̃X�e�[�W
		else if (input->Triger(DIK_LEFT) && stageNum > -1)
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
		selects[2].SpriteTransferVertexBuffer();
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
		selects[2].SpriteTransferVertexBuffer();
	}

	//�e�X�v���C�g�X�V
	stage[0].SpriteUpdate();
	stage[1].SpriteUpdate();
	stage[2].SpriteUpdate();
	stage[3].SpriteUpdate();
	stage[4].SpriteUpdate();
	selects[0].SpriteUpdate();
	selects[1].SpriteUpdate();
	selects[2].SpriteUpdate();
	toTutorial.SpriteUpdate();

	if (isPushStart && !startButtonEase_y.getIsActive())
	{
		isPushStart = false;

		//�^�C�g���ɖ߂�
		if (stageNum == -1)
		{
			startButton.size = { 340,50 };
			startButton.SpriteTransferVertexBuffer();
			scene = sceneType::title;
			return;
		}
		//�`���[�g���A���J�n
		else if (stageNum == 0)
		{
			//�v���C���[�̃��Z�b�g
			player_p->reset();
			isMoveText = true;
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
			player_p->update();
			Object3d_FBX::SetCamera(player_p->followCamera);

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

			player_p->update();
			Object3d_FBX::SetCamera(player_p->followCamera);

			scene = sceneType::play;
		}
	}
}

//�v���C��ʍX�V
void GameScene::Play_updata()
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
	Enemy::staticUpdata(player_p->getPlayerPos(), player_p->getPlayerFront(), player_p->isArive);

	if (!isCountDown && !isStartIcon)
	{
		//�Q�[�����ԃJ�E���g
		totalPlayFlameCount++;
	}

	//�p�[�e�B�N���̋��ʃJ������ݒ�
	SingleParticle::setCamera(player_p->followCamera);

	//�`���[�g���A��
	if (stageNum == 0)
	{
		tutorial();
		return;
	}

	if (input->Triger(DIK_LSHIFT))
	{
		player_p->playerHP = 0;
	}

	//�v���C���[�X�V
	player_p->update();
	checkHitManager::checkMissilesEnemy(&player_p->missilesList);

	//�G�l�~�[�X�V
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->updata();
		checkHitManager::checkPlayerEnemy(player_p.get(), newenemy.get());
		checkHitManager::chackPlayerEnemyBullet(player_p.get(), newenemy.get());
		checkHitManager::checkPlayerEnemyRampages(player_p.get(), newenemy.get());
		checkHitManager::checkBulletsEnemyRampage(&player_p->bulletsList, newenemy.get());
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
					player_p->addMissile(newenemy.get());

					newenemy->isSetMissile = true;

					break;
				}
			}
		}

		if (testBoss->isTargetSet && !testBoss->isSetMissile)
		{
			player_p->addMissile(testBoss.get());

			testBoss->isSetMissile = true;
		}

		targetnum = 0;
	}

	//�X�J�C�h�[��
	skySphere->Update();

	//�}�E�X�J�[�\����\��
	ShowCursor(false);

	//�G�����b�N�I������Ă��邩�ǂ���
	checkHitPlayerTarget();

	//�v���C���[�̒ʏ�e�����蔻��
	checkHitManager::checkBulletsEnemys(&player_p->bulletsList, &enemyList);
	checkHitManager::checkBulletsEnemyBullets(&player_p->bulletsList, &enemyList);

	//�ʏ�e�ƃ{�X�̓����蔻��
	checkHitManager::checkBulletsEnemy(&player_p->bulletsList, testBoss.get());
	checkHitManager::checkBulletsEnemybullet(&player_p->bulletsList, testBoss.get());

	//�v���C���[�ƃ{�X�̓����蔻��
	if (testBoss->getIsAppear() == false)
	{
		checkHitManager::checkPlayerEnemy(player_p.get(), testBoss.get());
		checkHitManager::chackPlayerEnemyBullet(player_p.get(), testBoss.get());
		checkHitManager::checkPlayerEnemyRampages(player_p.get(), testBoss.get());
		checkHitManager::checkBulletsEnemyRampage(&player_p->bulletsList, testBoss.get());
	}

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
		//�{�X�X�V
		testBoss->bossUpdate(player_p.get());

		for (std::unique_ptr<SingleSprite>& newsprite : bossHitPoints)
		{
			newsprite->SpriteUpdate();
		}
	}

	//�{�X��|����or�v���C���[�����񂾂烊�U���g
	if (isBoss && (!testBoss->isDraw))
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.SpriteTransferVertexBuffer();
		selects[2].position = { 640 - 150,540,0 };
		selectIconSizeX = 250;
		isSelectOrTitle = -1;
		ButtonEase_y.reSet();
		ButtonEase_x.reSet();

		//�X�e�[�W�N���A
		scene = sceneType::clear;

		bossHitPoints.clear();
	}

	if (!player_p->isArive && !player_p->isOverStaging && player_p->playerHP <= 0)
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.SpriteTransferVertexBuffer();
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
void GameScene::Result_updata()
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
		clearText.SpriteTransferVertexBuffer();
		clearText.SpriteUpdate();

		overText.position.x = 640;
		overText.position.y = resultScreen[0].position.y;
		overText.SpriteTransferVertexBuffer();
		overText.SpriteUpdate();

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

	resultScreen[0].SpriteUpdate();
	resultScreen[0].SpriteTransferVertexBuffer();
	resultScreen[1].SpriteUpdate();
	resultScreen[1].SpriteTransferVertexBuffer();

	//CLEAR�EOVER�̃e�L�X�g�̃C�[�W���O
	if (isTextEase)
	{
		clearText.position.x = 640;
		clearText.position.y = clearTextEase.easing();
		clearText.SpriteTransferVertexBuffer();
		clearText.SpriteUpdate();

		overText.position.x = 640;
		overText.position.y = overTextEase.easing();
		overText.SpriteTransferVertexBuffer();
		overText.SpriteUpdate();

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

	titleButton.SpriteUpdate();
	selectButton.SpriteUpdate();

	if (!isMoveSelectIcon)
	{
		//select���
		if (input->Triger(DIK_RIGHT) && isSelectOrTitle == -1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 - 150, 640 + 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
		//title���
		else if (input->Triger(DIK_LEFT) && isSelectOrTitle == 1)
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
		selects[2].SpriteTransferVertexBuffer();
	}
	selects[2].SpriteUpdate();

	if (input->Triger(DIK_SPACE) && !isPushTitle && !isMoveSelectIcon)
	{
		ButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 7, 0);
		ButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 200, 300);
		isPushTitle = true;
	}

	if (isPushTitle)
	{
		selects[2].size.y = ButtonEase_y.easing();
		selects[2].size.x = ButtonEase_x.easing();
		selects[2].SpriteTransferVertexBuffer();
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
			scene = sceneType::select;
		}
		else
		{
			startButton.size = { 340,50 };
			startButton.SpriteTransferVertexBuffer();
			isPushStart = false;
			scene = sceneType::title;
		}
	}
}

#pragma endregion �e�V�[���X�V

#pragma region �e�V�[���`��

//�^�C�g����ʕ`��
void GameScene::TitleDraw3d()
{
	if (scene != sceneType::title)
	{
		return;
	}
}
void GameScene::TitleDraw2d()
{
	if (scene != sceneType::title)
	{
		return;
	}

	resultScreen[0].DrawSprite(directx->cmdList.Get());
	gameTitle.DrawSprite(directx->cmdList.Get());
	startButton.DrawSprite(directx->cmdList.Get());
}

//�Z���N�g��ʕ`��
void GameScene::SelectDraw3d()
{
	if (scene != sceneType::select)
	{
		return;
	}
}
void GameScene::SelectDraw2d()
{
	if (scene != sceneType::select)
	{
		return;
	}

	resultScreen[0].DrawSprite(directx->cmdList.Get());
	stage[0].DrawSprite(directx->cmdList.Get());
	stage[1].DrawSprite(directx->cmdList.Get());
	stage[2].DrawSprite(directx->cmdList.Get());
	stage[3].DrawSprite(directx->cmdList.Get());
	stage[4].DrawSprite(directx->cmdList.Get());

	selects[0].DrawSprite(directx->cmdList.Get());
	selects[1].DrawSprite(directx->cmdList.Get());
	selects[2].DrawSprite(directx->cmdList.Get());

	if (!isTutorial && stageNum > 0)
	{
		toTutorial.DrawSprite(directx->cmdList.Get());
	}
}

//�v���C��ʕ`��
void GameScene::PlayDraw3d()
{
	if (scene != sceneType::play)
	{
		return;
	}

	skySphere->Draw(directx->cmdList.Get());

	//�v���C���[�`��
	player_p->draw3D(directx);

	//�G
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw3D(directx);
	}

	//�{�X�`��
	testBoss->draw3D(directx);
}
void GameScene::PlayDraw2d()
{
	if (scene != sceneType::play)
	{
		return;
	}

	player_p->draw2D(directx, targetnum);

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw2D(directx);
	}

	//�{�X�`��(2d)
	testBoss->draw2D(directx);

	if (isBoss)
	{
		for (int i = 0; i < testBoss->HP; i++)
		{
			bossHitPoints[i]->DrawSprite(directx->cmdList.Get());
		}
	}

	if (isCountDown)
	{
		countDownSprite[countDownNum].DrawSprite(directx->cmdList.Get());
	}

	if (isStartIcon)
	{
		playStart.DrawSprite(directx->cmdList.Get());
	}

	if (isTutorial)
	{
		if (isMoveText)
		{
			moveText.DrawSprite(directx->cmdList.Get());
		}
		else if (isShotText && !player_p->isStop)
		{
			shotText.DrawSprite(directx->cmdList.Get());
		}
		else if (isMissileText)
		{
			missileText.DrawSprite(directx->cmdList.Get());
		}
		else if (isShootingText)
		{
			shootingText.DrawSprite(directx->cmdList.Get());
		}
	}
}

//���U���g��ʕ`��
void GameScene::ResultDraw3d()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	skySphere->Draw(directx->cmdList.Get());

	//�v���C���[�`��
	player_p->draw3D(directx);

	//�{�X�`��
	testBoss->draw3D(directx);
}
void GameScene::ResultDraw2d()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	resultScreen[0].DrawSprite(directx->cmdList.Get());
	if (scene == sceneType::clear)
	{
		clearText.DrawSprite(directx->cmdList.Get());
	}
	else
	{
		overText.DrawSprite(directx->cmdList.Get());
	}
	resultScreen[1].DrawSprite(directx->cmdList.Get());

	if (isMoveScreen)
	{
		return;
	}
	titleButton.DrawSprite(directx->cmdList.Get());
	selectButton.DrawSprite(directx->cmdList.Get());
	selects[2].DrawSprite(directx->cmdList.Get());
}

#pragma endregion �e�V�[���`��

//�X�V
void GameScene::Updata()
{
	//�}�E�X���W�X�V
	MOUSE_POS = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };

	//�V�[���؂�ւ�

	//�^�C�g�����
	Title_updata();

	//�Z���N�g���
	Select_updata();

	//�v���C���
	Play_updata();

	//�N���A���
	if (scene == sceneType::clear || scene == sceneType::over)
	{
		Result_updata();
	}

	sample_back->SpriteUpdate();

	//camera->Updata();

	debugs_print();
}

//�w�i�X�v���C�g�`��
void GameScene::DrawBack()
{
	sample_back->DrawSprite(directx->cmdList.Get());
}

//�`��
void GameScene::Draw3D()
{
	//�Q�[�����V�[�����Ƃ̕`��
	TitleDraw3d();

	SelectDraw3d();

	PlayDraw3d();

	if (scene == sceneType::clear || scene == sceneType::over)
	{
		ResultDraw3d();
	}
}
void GameScene::Draw2D()
{
	TitleDraw2d();

	SelectDraw2d();

	PlayDraw2d();

	ResultDraw2d();

	//debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::checkHitPlayerTarget()
{
	//�ʏ�̓G
	checkHitManager::checkRockonEnemys(player_p.get(), &enemyList, targetnum);

	//�{�X
	checkHitManager::checkRockonEnemy(player_p.get(), testBoss.get(), targetnum);
}

void GameScene::countDown()
{
	if (!isCountDown && !isStartIcon)
	{
		return;
	}

	if (isCountDown)
	{
		countDownSprite[countDownNum].size = { countDownEase.easing(),countDownEase.easing() };
		countDownSprite[countDownNum].rotation -= 4;
		countDownSprite[countDownNum].SpriteTransferVertexBuffer();
		countDownSprite[countDownNum].SpriteUpdate();

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
		playStart.SpriteUpdate();

		if (startIconTime <= 0)
		{
			isStartIcon = false;
		}
	}

	//�X�^�[�g�̃J�E���g�_�E�����o���I������瓮����
	if (isCountDown || isStartIcon)
	{
		player_p->isStop = true;

		//�G
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->isStop = true;
		}

		testBoss->isStop = true;

		return;
	}
	else
	{
		player_p->isStop = false;

		//�G
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->isStop = false;
		}

		testBoss->isStop = false;
	}

	return;
}

void GameScene::tutorial()
{
	player_p->update();
	checkHitManager::checkMissilesEnemy(&player_p->missilesList);

	//���ʃJ�����𓮂�������G�o��
	if (player_p->cameraMoveCount >= 250 && isMoveText)
	{
		for (int i = 0; i < 20; i++)
		{
			//�G�@���X�g
			std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
			newenemy->init(enemyPattern::tutorial);
			newenemy->set({
			(float)(rand() % 50 - 25),
			(float)(rand() % 30 + 15),
			(float)(rand() % 50 - 25) });

			enemyList.push_back(std::move(newenemy));
		}

		isMoveText = false;
		isShotText = true;

		player_p->isNormalShot = true;
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
		player_p->isStop = true;
		player_p->isInvisible = 1;
		stagingCamera = new Camera;
		stagingCamera->SetEye({ 0,0,-30 });
		stagingCamera->SetTarget({ 0,30,0 });
		stagingCamera->Update();
		Object3d_FBX::SetCamera(stagingCamera);
	}
	else
	{
		player_p->isStop = false;
		player_p->isInvisible = -1;
		Object3d_FBX::SetCamera(player_p->followCamera);
	}

	//�ʏ�e -> �~�T�C��
	if (player_p->bulletsList.size() > 3)
	{
		isShotText = false;
		isMissileText = true;

		player_p->isHomingMissile = true;
	}

	//�~�T�C�� -> Lets Shooting!!!
	if (player_p->missilesList.size() > 3)
	{
		isMissileText = false;
		isShootingText = true;
	}

	moveText.SpriteUpdate();
	shotText.SpriteUpdate();
	missileText.SpriteUpdate();
	shootingText.SpriteUpdate();

	//�G�l�~�[�X�V
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->updata();
		checkHitManager::checkPlayerEnemy(player_p.get(), newenemy.get());
		checkHitManager::chackPlayerEnemyBullet(player_p.get(), newenemy.get());
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
					player_p->addMissile(newenemy.get());

					newenemy->isSetMissile = true;

					break;
				}
			}
		}

		if (testBoss->isTargetSet && !testBoss->isSetMissile)
		{
			player_p->addMissile(testBoss.get());

			testBoss->isSetMissile = true;
		}

		targetnum = 0;
	}

	//�X�J�C�h�[��
	skySphere->Update();

	//�}�E�X�J�[�\����\��
	ShowCursor(false);

	//�G�����b�N�I������Ă��邩�ǂ���
	checkHitPlayerTarget();

	//�v���C���[�̒ʏ�e�����蔻��
	checkHitManager::checkBulletsEnemys(&player_p->bulletsList, &enemyList);
	checkHitManager::checkBulletsEnemyBullets(&player_p->bulletsList, &enemyList);

	//�G�����ׂē|����or�v���C���[�����񂾂烊�U���g
	if (enemyList.size() <= 0 && !isMoveText && isShootingText)
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		isShootingText = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.SpriteTransferVertexBuffer();
		selects[2].position = { 640 - 150,540,0 };
		selectIconSizeX = 250;
		isSelectOrTitle = -1;
		ButtonEase_y.reSet();
		ButtonEase_x.reSet();
		scene = sceneType::clear;

		bossHitPoints.clear();
	}
	if (player_p->playerHP <= 0)
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		isShootingText = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.SpriteTransferVertexBuffer();
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
		return false;
	}

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
	player_p->reset();

	player_p->isNormalShot = true;
	player_p->isHomingMissile = true;

	isBoss = false;

	if (stageNum == 1)
	{
		stageLevel = stageNum + 2;

		//�G
		testBoss->changePattern(enemyPattern::chase);

		//�G�@���X�g
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::chase);
		}

		//�{�X�̐ݒ�
		testBoss->setHitPoint(stageLevel + 5);
	}
	else if (stageNum == 2)
	{
		stageLevel = stageNum + 2;

		//�G
		testBoss->changePattern(enemyPattern::shot);

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

		//�G
		testBoss->changePattern(enemyPattern::homing);

		//�G�@���X�g
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::rampage);
		}

		//�{�X�̐ݒ�
		testBoss->setHitPoint(stageLevel + 5);
	}

	for (int i = 0; i < testBoss->HP; i++)
	{
		std::unique_ptr<SingleSprite> newsprite = std::make_unique<SingleSprite>();
		newsprite->anchorpoint = { 0.5f,0 };
		newsprite->GenerateSprite("Enemy_HP.png");
		newsprite->size = { 40,60 };
		newsprite->position = { i * 30 + 660 - (30 * floorf(testBoss->HP / 2)),30,0 };
		newsprite->SpriteTransferVertexBuffer(false);

		bossHitPoints.push_back(std::move(newsprite));
	}

	return true;
}