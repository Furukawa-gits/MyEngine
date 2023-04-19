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
	delete(ground);
	delete(cameraobj);
	delete(model);
	delete(SkyModel);
	delete(groundModel);

	enemyList.clear();

	Enemy::staticDestroy();
	enemyBullet::staticDestroy();
}

//サウンドだけ読み込み関数
void GameScene::loadSounds()
{

}

//スプライト(シーン内で動かすもの)初期化
void GameScene::loadSprites()
{
	//背景
	sample_back = std::make_unique<SingleSprite>();
	sample_back->size = { 1280,720 };
	sample_back->generateSprite("sample_back.jpg");

	//タイトル
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

	//スタートボタン
	startButton.anchorpoint = { 0.5f,0.5f };
	startButton.size = { 430,50 };
	startButton.position = { 640,500,0 };
	startButton.generateSprite("startButton.png");

#pragma region //ステージアイコン
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
#pragma endregion //ステージアイコン

	//セレクト画面矢印
	selects[0].anchorpoint = { 0.5f,0.5f };
	selects[0].size = { 100,100 };
	selects[0].position = { 640 - 400,360,0 };
	selects[0].generateSprite("selectL.png");

	selects[1].anchorpoint = { 0.5f,0.5f };
	selects[1].size = { 100,100 };
	selects[1].position = { 640 + 400,360,0 };
	selects[1].generateSprite("selectR.png");

	//決定ボタン
	selects[2].anchorpoint = { 0.5f,0.5f };
	selects[2].size = { 150,5 };
	selects[2].position = { 640,420,0 };
	selects[2].generateSprite("selectIcon.png");

	toTutorial.anchorpoint = { 0.5f,0.5f };
	toTutorial.size = { 220,50 };
	toTutorial.position = { 640,470,0 };
	toTutorial.generateSprite("toTutorial.png");

#pragma region	//カウントダウンアイコン
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
#pragma endregion //カウントダウンアイコン

	//スタートテキスト
	playStart.anchorpoint = { 0.5f,0.5f };
	playStart.size = { 500,125 };
	playStart.position = { 640,360,0 };
	playStart.generateSprite("playstart.png");

	//リザルト画面
	resultScreen[0].size = { 1280,720 };
	resultScreen[0].generateSprite("black_color.png");

	resultScreen[1].size = { 1280,300 };
	resultScreen[1].generateSprite("black_color.png");

	//クリア・オーバー画面
	clearText.anchorpoint = { 0.5f,0.0f };
	clearText.size = { 640,100 };
	clearText.generateSprite("CLEAR_text.png");

	overText.anchorpoint = { 0.5f,0.0f };
	overText.size = { 640,100 };
	overText.generateSprite("OVER_text.png");

	//タイトルボタン
	titleButton.anchorpoint = { 0.5f,0.5f };
	titleButton.size = { 200,40 };
	titleButton.position = { 640 + 150,500,0 };
	titleButton.generateSprite("titleText.png");

	//セレクトボタン
	selectButton.anchorpoint = { 0.5f,0.5f };
	selectButton.size = { 200,40 };
	selectButton.position = { 640 - 150,500,0 };
	selectButton.generateSprite("selectText.png");

#pragma region	//チュートリアル用のテキスト
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
#pragma endregion //チュートリアル用のテキスト

#pragma region //ウェーブの表示
	enemyWaveBar.anchorpoint = { 0.5f,0.0f };
	enemyWaveBar.size = { 10,totalWaveBarLength };
	enemyWaveBar.position = { waveBarPosX,360 - totalWaveBarLength / 2,0 };
	enemyWaveBar.generateSprite("enemyWaveBar.png");

	playerWaveIcon.anchorpoint = { 0.5f,0.5f };
	playerWaveIcon.size = { 120,75 };
	playerWaveIcon.position = { waveBarPosX + 20,360 - totalWaveBarLength / 2,0 };
	playerWaveIcon.generateSprite("playerWaveIcon.png");
#pragma endregion //ウェーブの表示

	//ミニマップ
	miniMap.anchorpoint = { 0.5f,0.5f };
	miniMap.size = { 200,200 };
	miniMap.position = Enemy::miniMapPosition;
	miniMap.generateSprite("minimap.png");

	//高度メーター
	heightGauge.anchorpoint = { 0.5f,0.5f };
	heightGauge.size = { 40,200 };
	heightGauge.position = { Enemy::miniMapPosition.x + 120,Enemy::miniMapPosition.y,0 };
	heightGauge.generateSprite("playerheightbar.png");

	//プレイヤーの高度
	playerHeight.anchorpoint = { 0.5f,0.5f };
	playerHeight.size = { 32,5 };
	playerHeight.generateSprite("playerHPGauge.png");

	playerHeightIcon.anchorpoint = { 0.0f,0.5f };
	playerHeightIcon.size = { 44,17 };
	playerHeightIcon.generateSprite("playerHeightIcon.png");

	//マウスカーソル
	mouseCursur.anchorpoint = { 0.5f,0.5f };
	mouseCursur.size = { 20,20 };
	mouseCursur.generateSprite("Target.png");

	mouseCursurSub.anchorpoint = { 0.5f,0.5f };
	mouseCursurSub.size = { 20,20 };
	mouseCursurSub.generateSprite("Target.png");
}

//初期化
void GameScene::init(directX* directx, dxinput* input, Audio* audio)
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
	loadSounds();

	//スプライトクラス初期化
	SingleSprite::setStaticData(directx->dev.Get(), input);

	SingleParticle::particleStaticInit(directx, nullptr);

	//デバッグテキスト初期化
	debugtext.Init();

	//スプライト生成
	loadSprites();

	//ライト生成
	Light::Staticinitialize(directx->dev.Get());
	light = Light::Create();
	light->SetLightColor({ 1,1,1 });
	light->SetLightDir({ 0,-1,0,0 });

	//3dオブジェクト生成
	object3dFBX::setLight(light);

	object3dFBX::SetDevice(directx->dev.Get());

	object3dFBX::CreateGraphicsPipeline();

	object3dFBX::CreateGraphicsPipelineSimple();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");
	SkyModel = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");
	groundModel = FbxLoader::GetInstance()->LoadmodelFromFile("floar");

	//プレイヤー初期化
	playerPointer = std::make_unique<Player>();
	playerPointer->init(input, directx);

	skySphere = new object3dFBX;
	skySphere->initialize();
	skySphere->SetModel(SkyModel);
	skySphere->SetScale({ 8.0f,8.0f,8.0f });

	ground = new object3dFBX;
	ground->initialize();
	ground->SetModel(groundModel);
	ground->SetPosition(playerPointer->groundPosition);
	ground->SetScale({ 0.5f,0.5f,0.5f });

	cameraobj = new object3dFBX;
	cameraobj->initialize();
	cameraobj->SetModel(model);

	srand(time(NULL));

	//敵モデルの初期化
	Enemy::staticInit();

	//パーティクルの共通カメラを設定
	SingleParticle::setCamera(playerPointer->followCamera);

	//ボスの初期化
	Boss::staticInitBoss();
	testBoss = std::make_unique<Boss>();
	testBoss->bossInit();

	//ボス(ユニット)
	uniteBoss::uniteBossStaticInit(playerPointer.get());
	testUniteBoss = std::make_unique<uniteBoss>();
	testUniteBoss->uniteBossInit();

	//マウスカーソル非表示
	ShowCursor(false);

	//タイトルアニメーション準備
	titleDragEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, dragEaseStart, dragEaseEnd);
	titleShootEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, shootEaseStart, shootEaseEnd);
	isTitleAnimation = true;
	titleWhiteBackAlpha = 1.0f;
}

//デバッグテキスト
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

#pragma region 各シーン更新

//タイトル画面更新
void GameScene::titleUpdata()
{
	if (scene != sceneType::title)
	{
		return;
	}

	titleIconDrag.spriteTransferVertexBuffer();
	titleIconDrag.spriteUpdata();
	titleIconShoot.spriteTransferVertexBuffer();
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

	if ((input->Mouse_LeftTriger() || input->Triger(DIK_SPACE)) && !isPushStart && !isTitleAnimation)
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
		startButton.spriteTransferVertexBuffer();
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

//セレクト画面更新
void GameScene::selectUpdata()
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
		if (selects[1].isSpriteMouseInput() && stageNum < maxStageNum)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Cubic, 20, iconPos, iconPos - 300);
			stageNum++;
			isMoveStageIcon = true;
		}
		//前のステージ
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

	//セレクトアイコン通常拡縮
	if (!isPushStart)
	{
		selectIconSizeX -= 0.5f;

		if (selectIconSizeX <= 300)
		{
			selectIconSizeX = 350;
		}
		selects[2].size = { selectIconSizeX,10 };
		selects[2].spriteTransferVertexBuffer();
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
		selects[2].spriteTransferVertexBuffer();
	}

	//各スプライト更新
	for (int i = 0; i < 5; i++)
	{
		stage[i].spriteUpdata();
	}

	for (int i = 0; i < 3; i++)
	{
		selects[i].spriteTransferVertexBuffer();
		selects[i].spriteUpdata();
	}
	toTutorial.spriteUpdata();

	if (isPushStart && !startButtonEase_y.getIsActive())
	{
		isPushStart = false;

		//タイトルに戻る
		if (stageNum == -1)
		{
			startButton.size = { 340,50 };
			startButton.spriteTransferVertexBuffer();

			//タイトルアニメーション準備
			titleDragEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, dragEaseStart, dragEaseEnd);
			titleShootEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, shootEaseStart, shootEaseEnd);
			isTitleAnimation = true;
			titleWhiteBackAlpha = 1.0f;
			titleIconDrag.position.x = dragEaseStart;
			titleIconShoot.position.x = shootEaseStart;
			titleIconDrag.spriteTransferVertexBuffer();
			titleIconDrag.spriteUpdata();
			titleIconShoot.spriteTransferVertexBuffer();
			titleIconShoot.spriteUpdata();
			scene = sceneType::title;
			return;
		}
		//チュートリアル開始
		else if (stageNum == 0)
		{
			//プレイヤーのリセット
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
			playerPointer->updata();
			object3dFBX::SetCamera(playerPointer->followCamera);

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

			playerPointer->updata();
			object3dFBX::SetCamera(playerPointer->followCamera);

			scene = sceneType::play;
		}
	}
}

//プレイ画面更新
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

	//カウントダウン
	countDown();

	//敵に共通して必要なプレイヤーの情報を渡す
	Enemy::staticUpdata(playerPointer->getPlayerPos(), playerPointer->getPlayerFront(), playerPointer->isAlive);

	if (!isCountDown && !isStartIcon)
	{
		//ゲーム時間カウント
		totalPlayFlameCount++;
	}

	//パーティクルの共通カメラを設定
	SingleParticle::setCamera(playerPointer->followCamera);

	//スカイドーム更新
	skySphere->setRotMatrix(0, 0, skyShpereRotY);
	skySphere->updata();

	//地面更新
	ground->updata();

	//ミニマップ
	miniMap.spriteUpdata();

	//高度メーター更新
	//プレイヤーのy座標に補正をかけてpositionに代入
	float playerH = playerPointer->getPlayerPos().y - playerPointer->groundPosition.y;

	//スプライトの座標系に変更＆メーターの上がり具合を調整
	playerH = -(playerH * 0.3333f);

	//実際に表示する座標に変更
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

	//チュートリアル
	if (stageNum == 0)
	{
		tutorial();
		return;
	}

	if (input->Triger(DIK_LSHIFT))
	{
		playerPointer->playerHP = 0;
	}

	//プレイヤー更新
	playerPointer->updata();
	checkHitManager::checkMissilesEnemy(&playerPointer->missilesList);

	//エネミー更新
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->updata();
		checkHitManager::checkPlayerEnemy(playerPointer.get(), newenemy.get());
		checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), newenemy.get());
		checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, newenemy.get());
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

	//敵がロックオンされているかどうか
	checkHitPlayerTarget();

	//プレイヤーの通常弾当たり判定
	checkHitManager::checkBulletsEnemys(&playerPointer->bulletsList, &enemyList);

	//通常弾とボスの当たり判定
	checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, testBoss.get());

	//通常弾とユニットボス本体の当たり判定
	checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, testUniteBoss.get());

	//パーツ
	for (std::unique_ptr<uniteParts>& newparts : testUniteBoss->partsList)
	{
		checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, newparts.get());
	}

	//プレイヤーとボスの当たり判定
	if (!testBoss->getIsAppear() && stageNum < 3)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), testBoss.get());
		checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), testBoss.get());
		checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, testBoss.get());
	}

	if (!testUniteBoss->getIsAppear() && stageNum == 3)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), testUniteBoss.get());

		for (std::unique_ptr<uniteParts>& newparts : testUniteBoss->partsList)
		{
			checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), newparts.get());
			checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, newparts.get());
		}
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

		playerWaveIcon.position.y += nextWaveDis;

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
				//ユニットボス出現
				testUniteBoss->uniteBossSet();
				isBoss = true;
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
		if (stageNum < 3)
		{
			//ボス更新
			testBoss->bossUpdate(playerPointer.get());
		}
		else
		{
			//ユニットボス更新
			testUniteBoss->uniteBossUpdata();
		}
	}

	//ウェーブUIの更新
	enemyWaveBar.spriteTransferVertexBuffer();
	enemyWaveBar.spriteUpdata();

	for (std::unique_ptr<SingleSprite>& newsprite : enemyWaveIcons)
	{
		newsprite->spriteTransferVertexBuffer();
		newsprite->spriteUpdata();
	}

	playerWaveIcon.spriteTransferVertexBuffer();
	playerWaveIcon.spriteUpdata();

	//ボスを倒したorプレイヤーが死んだらリザルト
	if (isBoss && (!testBoss->isDraw) && (!testUniteBoss->isDraw))
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.spriteTransferVertexBuffer();
		selects[2].position = { 640 - 150,540,0 };
		selectIconSizeX = 250;
		isSelectOrTitle = -1;
		ButtonEase_y.reSet();
		ButtonEase_x.reSet();

		//ステージクリア
		scene = sceneType::clear;
	}

	if (!playerPointer->isAlive && !playerPointer->isOverStaging && playerPointer->playerHP <= 0)
	{
		isMoveScreen = true;
		isScreenEase = true;
		isTextEase = false;
		resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
		titleButton.spriteTransferVertexBuffer();
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
void GameScene::resultUpdata()
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
		clearText.spriteTransferVertexBuffer();
		clearText.spriteUpdata();

		overText.position.x = 640;
		overText.position.y = resultScreen[0].position.y;
		overText.spriteTransferVertexBuffer();
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
	resultScreen[0].spriteTransferVertexBuffer();
	resultScreen[1].spriteUpdata();
	resultScreen[1].spriteTransferVertexBuffer();

	//CLEAR・OVERのテキストのイージング
	if (isTextEase)
	{
		clearText.position.x = 640;
		clearText.position.y = clearTextEase.easing();
		clearText.spriteTransferVertexBuffer();
		clearText.spriteUpdata();

		overText.position.x = 640;
		overText.position.y = overTextEase.easing();
		overText.spriteTransferVertexBuffer();
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
	//背景とテキストを動かし終わっていなければボタンの処理は無し
	if (isMoveScreen)
	{
		return;
	}

	bool isTitleButton = titleButton.isSpriteMouseInput();
	bool isSelectButton = selectButton.isSpriteMouseInput();

	titleButton.spriteTransferVertexBuffer();
	titleButton.spriteUpdata();
	selectButton.spriteTransferVertexBuffer();
	selectButton.spriteUpdata();

	if (!isMoveSelectIcon)
	{
		//select画面
		if (selectButton.isMouseSelect && isSelectOrTitle == -1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 - 150, 640 + 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
		//title画面
		else if (titleButton.isMouseSelect && isSelectOrTitle == 1)
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
		selects[2].spriteTransferVertexBuffer();
	}
	selects[2].spriteUpdata();

	if ((isTitleButton || isSelectButton) && !isPushTitle && !isMoveSelectIcon)
	{
		ButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 7, 0);
		ButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 200, 300);
		isPushTitle = true;
	}

	if (isPushTitle)
	{
		selects[2].size.y = ButtonEase_y.easing();
		selects[2].size.x = ButtonEase_x.easing();
		selects[2].spriteTransferVertexBuffer();
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
			startButton.spriteTransferVertexBuffer();
			isPushStart = false;
			selects[2].position = { 640,420,0 };
			stageNum = 0;
			stage[0].position.x = 640 - 300;

			//タイトルアニメーション準備
			titleDragEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, dragEaseStart, dragEaseEnd);
			titleShootEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, shootEaseStart, shootEaseEnd);
			isTitleAnimation = true;
			titleWhiteBackAlpha = 1.0f;
			titleIconDrag.position.x = dragEaseStart;
			titleIconShoot.position.x = shootEaseStart;
			titleIconDrag.spriteTransferVertexBuffer();
			titleIconDrag.spriteUpdata();
			titleIconShoot.spriteTransferVertexBuffer();
			titleIconShoot.spriteUpdata();

			scene = sceneType::title;
		}
	}
}

#pragma endregion 各シーン更新

#pragma region 各シーン描画

//タイトル画面描画
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

//セレクト画面描画
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

	mouseCursurSub.drawSprite(directx->cmdList.Get());
	mouseCursur.drawSprite(directx->cmdList.Get());
}

//プレイ画面描画
void GameScene::playDraw3d()
{
	if (scene != sceneType::play)
	{
		return;
	}

	ground->Draw(directx->cmdList.Get());
	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	playerPointer->draw3D(directx);

	//敵
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw3D(directx);
	}

	//ボス描画
	testBoss->bossDraw3D(directx);

	testUniteBoss->uniteBossDraw3d(directx);
}
void GameScene::playDraw2d()
{
	if (scene != sceneType::play)
	{
		return;
	}

	if (stageNum > 0)
	{
		playerPointer->draw2D(directx, targetnum);
	}

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw2D(directx);
	}

	//ボス描画(2d)
	testBoss->bossDraw2D(directx);

	testUniteBoss->uniteBossDraw2d(directx);

	if (isCountDown)
	{
		countDownSprite[countDownNum].drawSprite(directx->cmdList.Get());
	}

	if (isStartIcon)
	{
		playStart.drawSprite(directx->cmdList.Get());
	}

	//ウェーブ描画
	drawNowWave();

	//ミニマップ＆高度メーター描画
	drawPositionUI();

	//チュートリアルテキストの都合上、プレイヤーの描画順は分けている
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

//リザルト画面描画
void GameScene::resultDraw3d()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	ground->Draw(directx->cmdList.Get());
	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	playerPointer->draw3D(directx);

	//ボス描画
	testBoss->bossDraw3D(directx);
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

#pragma endregion 各シーン描画

//更新
void GameScene::updata()
{
	//マウス座標更新
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y,0.0f };

	//マウスカーソル更新
	mouseCursur.position = MOUSE_POS;
	mouseCursur.rotation += 2.0f;
	mouseCursur.spriteTransferVertexBuffer();
	mouseCursur.spriteUpdata();
	mouseCursurSub.position = MOUSE_POS;
	mouseCursurSub.rotation -= 4.0f;
	mouseCursurSub.spriteTransferVertexBuffer();
	mouseCursurSub.spriteUpdata();

	//ライト更新
	light->Update();

	//シーン切り替え

	//タイトル画面
	titleUpdata();

	//セレクト画面
	selectUpdata();

	//プレイ画面
	playUpdata();

	//クリア画面
	if (scene == sceneType::clear || scene == sceneType::over)
	{
		resultUpdata();
	}

	sample_back->spriteUpdata();

	debugsPrint();
}

//背景スプライト描画
void GameScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

//描画
void GameScene::draw3D()
{
	//ゲーム内シーンごとの描画
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

	//高度メーター
	heightGauge.drawSprite(directx->cmdList.Get());
	playerHeight.drawSprite(directx->cmdList.Get());
	playerHeightIcon.drawSprite(directx->cmdList.Get());

	//ミニマップ
	miniMap.drawSprite(directx->cmdList.Get());

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->drawMiniMapIcon(directx);
	}

	testBoss->drawMiniMapIcon(directx);

	testUniteBoss->drawMiniMapIcon(directx);

	playerPointer->drawMiniMapIcon(directx);
}

void GameScene::checkHitPlayerTarget()
{
	//通常の敵
	checkHitManager::checkRockonEnemys(playerPointer.get(), &enemyList, targetnum);

	//ボス
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
		countDownSprite[countDownNum].spriteTransferVertexBuffer();
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

	//スタートのカウントダウン演出が終わったら動ける
	if (isCountDown || isStartIcon)
	{
		playerPointer->isStop = true;

		//敵
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

		//敵
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
	checkHitManager::checkMissilesEnemy(&playerPointer->missilesList);

	//一定量カメラを動かしたら敵出現
	if (playerPointer->cameraMoveCount >= 250 && isMoveText)
	{
		isMoveText = false;
		isBoostText = true;

		playerPointer->isBoostTutorial = true;
	}

	//3回ブーストしたら敵出現
	if (!playerPointer->isBoost && playerPointer->boostCount > 2 && isBoostText)
	{
		for (int i = 0; i < 20; i++)
		{
			//敵　リスト
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

	//通常弾 -> ミサイル
	if (playerPointer->normalShotCount > 3)
	{
		isShotText = false;
		isMissileText = true;

		playerPointer->isHomingMissile = true;
	}

	//ミサイル -> Lets Shooting!!!
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

	//エネミー更新
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->updata();
		checkHitManager::checkPlayerEnemy(playerPointer.get(), newenemy.get());
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

	//敵がロックオンされているかどうか
	checkHitPlayerTarget();

	//プレイヤーの通常弾当たり判定
	checkHitManager::checkBulletsEnemys(&playerPointer->bulletsList, &enemyList);

	//敵をすべて倒したorプレイヤーが死んだらリザルト
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
		titleButton.spriteTransferVertexBuffer();
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
		titleButton.spriteTransferVertexBuffer();
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

	//チュートリアルのテキストを表示しないようにする
	isMoveText = false;
	isBoostText = false;
	isShotText = false;
	isMissileText = false;
	isShootingText = false;

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
	playerPointer->reset();

	isBoss = false;

	if (stageNum == 1)
	{
		stageLevel = stageNum + 2;

		//敵
		testBoss->changePattern(enemyPattern::chase);

		//敵　リスト
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::shot);
		}

		//ボスの設定
		testBoss->setHitPoint(stageLevel + 5);
	}
	else if (stageNum == 2)
	{
		stageLevel = stageNum + 2;

		//敵
		testBoss->changePattern(enemyPattern::rampage);

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

		//敵　リスト
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::rampage);
		}
	}

	//アイコン同士の距離を計算
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
		newicon->spriteTransferVertexBuffer();
		enemyWaveIcons.push_back(std::move(newicon));
	}

	std::unique_ptr<SingleSprite> newicon = std::make_unique<SingleSprite>();
	newicon->anchorpoint = { 0.5f,0.5f };
	newicon->size = { 100,100 };
	newicon->position = { waveBarPosX,(360 - totalWaveBarLength / 2) + totalWaveBarLength,0 };
	newicon->generateSprite("bossWaveIcon.png");
	newicon->spriteTransferVertexBuffer();
	enemyWaveIcons.push_back(std::move(newicon));

	return true;
}