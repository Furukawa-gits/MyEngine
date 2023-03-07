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

//サウンドだけ読み込み関数
void GameScene::Load_sounds()
{

}

//スプライト(シーン内で動かすもの)初期化
void GameScene::Load_Sprites()
{
	//背景
	sample_back = std::make_unique<SingleSprite>();
	sample_back->size = { 1280,720 };
	sample_back->GenerateSprite("sample_back.jpg");

	//タイトル
	gameTitle.anchorpoint = { 0.5f,0.5f };
	gameTitle.size = { 500,220 };
	gameTitle.position = { 640,200,0 };
	gameTitle.GenerateSprite("DragShoot.png");

	//スタートボタン
	startButton.anchorpoint = { 0.5f,0.5f };
	startButton.size = { 340,50 };
	startButton.GenerateSprite("startButton.png");
	startButton.position = { 640,500,0 };
	startButton.SpriteTransferVertexBuffer();

#pragma region //ステージアイコン０～２
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
#pragma endregion //ステージアイコン０～２

	//セレクト画面矢印
	selects[0].anchorpoint = { 0.5f,0.5f };
	selects[0].size = { 100,100 };
	selects[0].position = { 640 - 400,360,0 };
	selects[0].GenerateSprite("selectL.png");

	selects[1].anchorpoint = { 0.5f,0.5f };
	selects[1].size = { 100,100 };
	selects[1].position = { 640 + 400,360,0 };
	selects[1].GenerateSprite("selectR.png");

	//決定ボタン
	selects[2].anchorpoint = { 0.5f,0.5f };
	selects[2].size = { 150,5 };
	selects[2].position = { 640,420,0 };
	selects[2].GenerateSprite("selectIcon.png");

	toTutorial.anchorpoint = { 0.5f,0.5f };
	toTutorial.size = { 220,50 };
	toTutorial.position = { 640,470,0 };
	toTutorial.GenerateSprite("toTutorial.png");

#pragma region	//カウントダウンアイコン
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
#pragma endregion //カウントダウンアイコン

	playStart.anchorpoint = { 0.5f,0.5f };
	playStart.size = { 500,125 };
	playStart.position = { 640,360,0 };
	playStart.GenerateSprite("playstart.png");

	//リザルト画面
	resultScreen[0].size = { 1280,720 };
	resultScreen[0].GenerateSprite("black_color.png");

	resultScreen[1].size = { 1280,300 };
	resultScreen[1].GenerateSprite("black_color.png");

	//クリア・オーバー画面
	clearText.anchorpoint = { 0.5f,0.0f };
	clearText.size = { 640,100 };
	clearText.GenerateSprite("CLEAR_text.png");

	overText.anchorpoint = { 0.5f,0.0f };
	overText.size = { 640,100 };
	overText.GenerateSprite("OVER_text.png");

	//タイトルボタン
	titleButton.anchorpoint = { 0.5f,0.5f };
	titleButton.size = { 150,50 };
	titleButton.GenerateSprite("titleText.png");
	titleButton.position = { 640 + 150,500,0 };
	titleButton.SpriteTransferVertexBuffer();

	//セレクトボタン
	selectButton.anchorpoint = { 0.5f,0.5f };
	selectButton.size = { 200,50 };
	selectButton.GenerateSprite("selectText.png");
	selectButton.position = { 640 - 150,500,0 };
	selectButton.SpriteTransferVertexBuffer();

	//チュートリアル用のテキスト
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

//初期化
void GameScene::Init(directX* directx, dxinput* input, Audio* audio)
{
	//nullチェック
	assert(directx);
	assert(input);
	assert(audio);

	//基幹部分をリンク
	this->directx = directx;
	this->input = input;
	this->audio = audio;

	//音読み込み
	Load_sounds();

	//スプライトクラス初期化
	SingleSprite::SetStaticData(directx->dev.Get());

	SingleParticle::particleStaticInit(directx, nullptr);

	//デバッグテキスト初期化
	debugtext.Init();

	//スプライト生成
	Load_Sprites();

	//3dオブジェクト生成
	Object3d_FBX::SetDevice(directx->dev.Get());

	Object3d_FBX::CreateGraphicsPipeline();

	Object3d_FBX::CreateGraphicsPipelineSimple();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");
	SkyModel = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");

	//プレイヤー初期化
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

	//敵モデルの初期化
	Enemy::staticInit();

	//パーティクルの共通カメラを設定
	SingleParticle::setCamera(player_p->followCamera);

	//ボスの初期化
	Boss::staticInitBoss();
	testBoss = std::make_unique<Boss>();
	testBoss->bossInit();

	//ボス(ユニット)
	uniteBoss::uniteBossStaticInit(player_p.get());
	testUniteBoss = std::make_unique<uniteBoss>();
	testUniteBoss->uniteBossInit();
}

//デバッグテキスト
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

#pragma region 各シーン更新

//タイトル画面更新
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

//セレクト画面更新
void GameScene::Select_updata()
{
	if (scene != sceneType::select)
	{
		return;
	}

	//ステージアイコンが動いていなければセレクトボタン受付
	if (!isMoveStageIcon)
	{
		float iconPos = stage[0].position.x;

		//次のステージ
		if (input->Triger(DIK_RIGHT) && stageNum < maxStageNum)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Cubic, 20, iconPos, iconPos - 300);
			stageNum++;
			isMoveStageIcon = true;
		}
		//前のステージ
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

	//セレクトアイコン通常拡縮
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

	//ステージ選択ムーブ中でなければ決定
	if (input->Triger(DIK_SPACE) && !isMoveStageIcon && !isPushStart)
	{
		targetnum = 0;

		startButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 20, 0);
		startButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 300, 350);
		isPushStart = true;
	}

	//スタートボタン拡縮
	if (isPushStart)
	{
		selects[2].size.y = startButtonEase_y.easing();
		selects[2].size.x = startButtonEase_x.easing();
		selects[2].SpriteTransferVertexBuffer();
	}

	//各スプライト更新
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

		//タイトルに戻る
		if (stageNum == -1)
		{
			startButton.size = { 340,50 };
			startButton.SpriteTransferVertexBuffer();
			scene = sceneType::title;
			return;
		}
		//チュートリアル開始
		else if (stageNum == 0)
		{
			//プレイヤーのリセット
			player_p->reset();
			isMoveText = true;
			isShotText = false;
			isMissileText = false;
			isShootingText = false;
			isBoss = false;
			//スタートのカウントダウンを設定
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
			//ステージ読み込み
			isLoadStage = loadStage();
		}

		if (isLoadStage)
		{
			//スタートのカウントダウンを設定
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

//プレイ画面更新
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

	//カウントダウン
	countDown();

	//敵に共通して必要なプレイヤーの情報を渡す
	Enemy::staticUpdata(player_p->getPlayerPos(), player_p->getPlayerFront(), player_p->isArive);

	if (!isCountDown && !isStartIcon)
	{
		//ゲーム時間カウント
		totalPlayFlameCount++;
	}

	//パーティクルの共通カメラを設定
	SingleParticle::setCamera(player_p->followCamera);

	//チュートリアル
	if (stageNum == 0)
	{
		tutorial();
		return;
	}

	if (input->Triger(DIK_LSHIFT))
	{
		player_p->playerHP = 0;
	}

	//プレイヤー更新
	player_p->update();
	checkHitManager::checkMissilesEnemy(&player_p->missilesList);

	//エネミー更新
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

	//ホーミング弾発射
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

	//スカイドーム
	skySphere->Update();

	//マウスカーソル非表示
	ShowCursor(false);

	//敵がロックオンされているかどうか
	checkHitPlayerTarget();

	//プレイヤーの通常弾当たり判定
	checkHitManager::checkBulletsEnemys(&player_p->bulletsList, &enemyList);
	checkHitManager::checkBulletsEnemyBullets(&player_p->bulletsList, &enemyList);

	//通常弾とボスの当たり判定
	checkHitManager::checkBulletsEnemy(&player_p->bulletsList, testBoss.get());
	checkHitManager::checkBulletsEnemybullet(&player_p->bulletsList, testBoss.get());

	//プレイヤーとボスの当たり判定
	if (testBoss->getIsAppear() == false)
	{
		checkHitManager::checkPlayerEnemy(player_p.get(), testBoss.get());
		checkHitManager::chackPlayerEnemyBullet(player_p.get(), testBoss.get());
		checkHitManager::checkPlayerEnemyRampages(player_p.get(), testBoss.get());
		checkHitManager::checkBulletsEnemyRampage(&player_p->bulletsList, testBoss.get());
	}

	//死んでいる雑魚敵をカウント
	int count = 0;
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		if (!newenemy->isDraw)
		{
			count++;
		}
	}

	//すべての雑魚敵が死んでいたら次のウェーブ
	if (enemyList.size() <= 0 && nowStageLevel < stageLevel)
	{
		nowStageLevel++;

		//最後のウェーブならボス戦
		if (nowStageLevel == stageLevel && !isBoss)
		{
			if (stageNum < 3)
			{
				//ボス出現
				testBoss->bossSet({ 0,5,0 });
				isBoss = true;
			}
			else
			{

			}
		}
		//でなければ次の敵軍
		else
		{
			//次ウェーブ
			enemySpawnNum = (rand() % 4) + 3 + stageNum;	//敵出現数
			int nextType = (rand() % 4) + 1;				//敵の種類

			for (int i = 0; i < enemySpawnNum; i++)
			{
				//敵　リスト
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
		//ボス更新
		testBoss->bossUpdate(player_p.get());

		for (std::unique_ptr<SingleSprite>& newsprite : bossHitPoints)
		{
			newsprite->SpriteUpdate();
		}
	}

	//ボスを倒したorプレイヤーが死んだらリザルト
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

		//ステージクリア
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

		//ゲームオーバー
		scene = sceneType::over;
	}
}

//リザルト画面更新
void GameScene::Result_updata()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	//画面背景のイージング
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

	//CLEAR・OVERのテキストのイージング
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
	//背景とテキストを動かし終わっていなければボタンの処理は無し
	if (isMoveScreen)
	{
		return;
	}

	titleButton.SpriteUpdate();
	selectButton.SpriteUpdate();

	if (!isMoveSelectIcon)
	{
		//select画面
		if (input->Triger(DIK_RIGHT) && isSelectOrTitle == -1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 - 150, 640 + 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
		//title画面
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

	//セレクトアイコン通常拡縮
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

	//タイトル画面準備
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

#pragma endregion 各シーン更新

#pragma region 各シーン描画

//タイトル画面描画
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

//セレクト画面描画
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

//プレイ画面描画
void GameScene::PlayDraw3d()
{
	if (scene != sceneType::play)
	{
		return;
	}

	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	player_p->draw3D(directx);

	//敵
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw3D(directx);
	}

	//ボス描画
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

	//ボス描画(2d)
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

//リザルト画面描画
void GameScene::ResultDraw3d()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	player_p->draw3D(directx);

	//ボス描画
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

#pragma endregion 各シーン描画

//更新
void GameScene::Updata()
{
	//マウス座標更新
	MOUSE_POS = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };

	//シーン切り替え

	//タイトル画面
	Title_updata();

	//セレクト画面
	Select_updata();

	//プレイ画面
	Play_updata();

	//クリア画面
	if (scene == sceneType::clear || scene == sceneType::over)
	{
		Result_updata();
	}

	sample_back->SpriteUpdate();

	//camera->Updata();

	debugs_print();
}

//背景スプライト描画
void GameScene::DrawBack()
{
	sample_back->DrawSprite(directx->cmdList.Get());
}

//描画
void GameScene::Draw3D()
{
	//ゲーム内シーンごとの描画
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
	//通常の敵
	checkHitManager::checkRockonEnemys(player_p.get(), &enemyList, targetnum);

	//ボス
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

	//スタートのカウントダウン演出が終わったら動ける
	if (isCountDown || isStartIcon)
	{
		player_p->isStop = true;

		//敵
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

		//敵
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

	//一定量カメラを動かしたら敵出現
	if (player_p->cameraMoveCount >= 250 && isMoveText)
	{
		for (int i = 0; i < 20; i++)
		{
			//敵　リスト
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

	//敵が出現演出途中ならカメラをセットする
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

	//通常弾 -> ミサイル
	if (player_p->bulletsList.size() > 3)
	{
		isShotText = false;
		isMissileText = true;

		player_p->isHomingMissile = true;
	}

	//ミサイル -> Lets Shooting!!!
	if (player_p->missilesList.size() > 3)
	{
		isMissileText = false;
		isShootingText = true;
	}

	moveText.SpriteUpdate();
	shotText.SpriteUpdate();
	missileText.SpriteUpdate();
	shootingText.SpriteUpdate();

	//エネミー更新
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

	//ホーミング弾発射
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

	//スカイドーム
	skySphere->Update();

	//マウスカーソル非表示
	ShowCursor(false);

	//敵がロックオンされているかどうか
	checkHitPlayerTarget();

	//プレイヤーの通常弾当たり判定
	checkHitManager::checkBulletsEnemys(&player_p->bulletsList, &enemyList);
	checkHitManager::checkBulletsEnemyBullets(&player_p->bulletsList, &enemyList);

	//敵をすべて倒したorプレイヤーが死んだらリザルト
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
		//敵　リスト
		std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
		newenemy->init(enemyPattern::shot);
		newenemy->set({
		(float)(rand() % 50 - 25),
		(float)(rand() % 30 + 15),
		(float)(rand() % 50 - 25) });

		enemyList.push_back(std::move(newenemy));
	}

	//プレイヤーのリセット
	player_p->reset();

	player_p->isNormalShot = true;
	player_p->isHomingMissile = true;

	isBoss = false;

	if (stageNum == 1)
	{
		stageLevel = stageNum + 2;

		//敵
		testBoss->changePattern(enemyPattern::chase);

		//敵　リスト
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::chase);
		}

		//ボスの設定
		testBoss->setHitPoint(stageLevel + 5);
	}
	else if (stageNum == 2)
	{
		stageLevel = stageNum + 2;

		//敵
		testBoss->changePattern(enemyPattern::shot);

		//敵　リスト
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::shot);
		}

		//ボスの設定
		testBoss->setHitPoint(stageLevel + 5);
	}
	else if (stageNum == 3)
	{
		stageLevel = stageNum + 2;

		//敵
		testBoss->changePattern(enemyPattern::homing);

		//敵　リスト
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::rampage);
		}

		//ボスの設定
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