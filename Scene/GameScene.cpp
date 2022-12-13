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

	//�X�e�[�W�A�C�R���P�`�Q
	stage1.anchorpoint = { 0.5f,0.5f };
	stage1.size = { 128,128 };
	stage1.position = { 0,360,0 };
	stage1.GenerateSprite("count1.png");

	stage2.anchorpoint = { 0.5f,0.5f };
	stage2.size = { 128,128 };
	stage2.position = { 0,360,0 };
	stage2.GenerateSprite("count2.png");

	//�Z���N�g��ʖ��
	selects[0].anchorpoint = { 0.5f,0.5f };
	selects[0].size = { 100,100 };
	selects[0].position = { 640 - 400,360,0 };
	selects[0].GenerateSprite("selectL.png");

	selects[1].anchorpoint = { 0.5f,0.5f };
	selects[1].size = { 100,100 };
	selects[1].position = { 640 + 400,360,0 };
	selects[1].GenerateSprite("selectR.png");

	//�J�E���g�_�E���A�C�R��
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
	titleButton.size = { 315,50 };
	titleButton.GenerateSprite("toTitle.png");
	titleButton.position = { 640,500,0 };
	titleButton.SpriteTransferVertexBuffer();
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
	testBoss = std::make_unique<Boss>();
	testBoss->bossInit();
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

	startButton.SpriteUpdate();
	resultScreen[0].SpriteUpdate();
	gameTitle.SpriteUpdate();

	if (isPushStart && !startButtonEase_y.getIsActive())
	{
		stage1.position.x = 640;
		isMoveStageIcon = false;
		stageNum = 1;
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

	if (!isMoveStageIcon)
	{
		float iconPos = stage1.position.x;

		//���̃X�e�[�W
		if (input->Triger(DIK_RIGHT) && stageNum < 2)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Cubic, 20, iconPos, iconPos - 300);
			stageNum++;
			isMoveStageIcon = true;
		}
		//�O�̃X�e�[�W
		else if (input->Triger(DIK_LEFT) && stageNum > 1)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Quadratic, 20, iconPos, iconPos + 300);
			stageNum--;
			isMoveStageIcon = true;
		}
	}
	else
	{
		stage1.position.x = stageIconEase.easing();

		if (!stageIconEase.getIsActive())
		{
			isMoveStageIcon = false;
		}
	}

	stage2.position.x = stage1.position.x + 300;

	stage1.SpriteUpdate();
	stage2.SpriteUpdate();

	selects[0].SpriteUpdate();
	selects[1].SpriteUpdate();

	if (input->Triger(DIK_SPACE) && !isMoveStageIcon)
	{
		targetnum = 0;

		for (int i = 0; i < maxEnemyNum; i++)
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

		player_p->reset();

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
		else
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
		titleButton.size = { 315,50 };

		nowStageLevel = 1;

		player_p->update();

		scene = sceneType::play;
	}
}

//�v���C��ʍX�V
void GameScene::Play_updata()
{
	if (scene != sceneType::play)
	{
		return;
	}

	//�J�E���g�_�E��
	countDown();

	//�v���C���[�X�V
	player_p->update();
	checkHitManager::checkMissilesEnemy(&player_p->missilesList);


	//���Z�b�g
	if (input->push(DIK_R))
	{
	}

	//�G�l�~�[�X�V
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->update(player_p->playerObject->getPosition());
		checkHitManager::checkPlayerEnemy(player_p.get(), newenemy.get());
		checkHitManager::chackPlayerEnemyBullet(player_p.get(), newenemy.get());
	}

	//�{�X�X�V
	testBoss->bossUpdate(player_p.get());

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
	if (enemyList.size() <= 0 && !isBoss)
	{
		nowStageLevel++;

		if (nowStageLevel == stageLevel)
		{
			//�{�X�o��
		}
		else
		{
			//���E�F�[�u�̓G�o��
		}

		//�{�X�o��
		testBoss->bossSet({ 0,5,0 });
		isBoss = true;
	}

	if (isBoss)
	{
		for (std::unique_ptr<SingleSprite>& newsprite : bossHitPoints)
		{
			newsprite->SpriteUpdate();
		}
	}

	//�{�X��|����or�v���C���[�����񂾂烊�U���g
	if ((isBoss && !testBoss->isDraw))
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.size = { 315,50 };
		titleButton.SpriteTransferVertexBuffer();
		titleButtonEase_y.reSet();
		titleButtonEase_x.reSet();
		scene = sceneType::clear;

		bossHitPoints.clear();
	}

	if (player_p->playerHP <= 0)
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.size = { 315,50 };
		titleButton.SpriteTransferVertexBuffer();
		titleButtonEase_y.reSet();
		titleButtonEase_x.reSet();
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
		titleButton.SpriteUpdate();
	}

	titleButton.SpriteUpdate();

	if (input->Triger(DIK_SPACE) && !isMoveScreen)
	{
		titleButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 50, 0);
		titleButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 315, 400);
		isPushTitle = true;
	}

	if (isPushTitle)
	{
		titleButton.size.y = titleButtonEase_y.easing();
		titleButton.size.x = titleButtonEase_x.easing();
		titleButton.SpriteTransferVertexBuffer();
	}

	titleButton.SpriteUpdate();

	if (isPushTitle && !titleButtonEase_y.getIsActive())
	{
		startButton.size = { 340,50 };
		startButton.SpriteTransferVertexBuffer();
		isPushStart = false;
		scene = sceneType::title;
	}
}

#pragma endregion �e�V�[���X�V

#pragma region �e�V�[���`��

//�^�C�g����ʕ`��
void GameScene::Title_draw()
{
	if (scene != sceneType::title)
	{
		return;
	}
}

//���N�Ɖ�ʕ`��
void GameScene::Select_draw()
{
	if (scene != sceneType::select)
	{
		return;
	}
}

//�v���C��ʕ`��
void GameScene::Play_draw()
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

//���U���g��ʕ`��
void GameScene::Result_draw()
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

#pragma endregion �e�V�[���`��

//�X�V
void GameScene::Updata()
{
	//�}�E�X���W�X�V
	MOUSE_POS = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };

	//�Q�[�����ԃJ�E���g
	game_time++;

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
	Title_draw();

	Select_draw();

	Play_draw();

	if (scene == sceneType::clear || scene == sceneType::over)
	{
		Result_draw();
	}
}

void GameScene::Draw2D()
{
	if (scene == sceneType::title)
	{
		resultScreen[0].DrawSprite(directx->cmdList.Get());
		gameTitle.DrawSprite(directx->cmdList.Get());
		startButton.DrawSprite(directx->cmdList.Get());
	}

	if (scene == sceneType::select)
	{
		stage1.DrawSprite(directx->cmdList.Get());
		stage2.DrawSprite(directx->cmdList.Get());

		selects[0].DrawSprite(directx->cmdList.Get());
		selects[1].DrawSprite(directx->cmdList.Get());
	}

	if (scene == sceneType::play)
	{
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

		player_p->draw2D(directx);

		if (isCountDown)
		{
			countDownSprite[countDownNum].DrawSprite(directx->cmdList.Get());
		}

		if (isStartIcon)
		{
			playStart.DrawSprite(directx->cmdList.Get());
		}
	}

	if (scene == sceneType::clear)
	{
		resultScreen[0].DrawSprite(directx->cmdList.Get());
		clearText.DrawSprite(directx->cmdList.Get());
		resultScreen[1].DrawSprite(directx->cmdList.Get());

		if (!isMoveScreen)
		{
			titleButton.DrawSprite(directx->cmdList.Get());
		}
	}

	if (scene == sceneType::over)
	{
		resultScreen[0].DrawSprite(directx->cmdList.Get());
		overText.DrawSprite(directx->cmdList.Get());
		resultScreen[1].DrawSprite(directx->cmdList.Get());

		if (!isMoveScreen)
		{
			titleButton.DrawSprite(directx->cmdList.Get());
		}
	}
	//debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::checkHitPlayerTarget()
{
	if (!player_p->isRockOn)
	{
		return;
	}

	if (!input->Mouse_LeftPush())
	{
		return;
	}

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