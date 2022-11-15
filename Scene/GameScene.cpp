#include"GameScene.h"
#include"../FbxLoder/Object3d_FBX.h"
#include<cassert>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	delete(object);
	delete(model);
	delete(skySphere);
	delete(cameraobj);
}

//サウンドだけ読み込み関数
void GameScene::Load_sounds()
{

}

//スプライト(シーン内で動かすもの)初期化
void GameScene::Load_Sprites()
{
	//背景
	sample_back.size = { 1280,720 };
	sample_back.GenerateSprite("sample_back.jpg");

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

	//ステージアイコン１～２
	stage1.anchorpoint = { 0.5f,0.5f };
	stage1.size = { 128,128 };
	stage1.position = { 0,360,0 };
	stage1.GenerateSprite("count1.png");

	stage2.anchorpoint = { 0.5f,0.5f };
	stage2.size = { 128,128 };
	stage2.position = { 0,360,0 };
	stage2.GenerateSprite("count2.png");

	//セレクト画面矢印
	selects[0].anchorpoint = { 0.5f,0.5f };
	selects[0].size = { 100,100 };
	selects[0].position = { 640 - 400,360,0 };
	selects[0].GenerateSprite("selectL.png");

	selects[1].anchorpoint = { 0.5f,0.5f };
	selects[1].size = { 100,100 };
	selects[1].position = { 640 + 400,360,0 };
	selects[1].GenerateSprite("selectR.png");

	//カウントダウンアイコン
	countDown[0].anchorpoint = { 0.5f,0.5f };
	countDown[0].size = { 100,100 };
	countDown[0].position = { 640,360,0 };
	countDown[0].GenerateSprite("count3.png");

	countDown[1].anchorpoint = { 0.5f,0.5f };
	countDown[1].size = { 100,100 };
	countDown[1].position = { 640,360,0 };
	countDown[1].GenerateSprite("count2.png");

	countDown[2].anchorpoint = { 0.5f,0.5f };
	countDown[2].size = { 100,100 };
	countDown[2].position = { 640,360,0 };
	countDown[2].GenerateSprite("count1.png");

	playStart.anchorpoint = { 0.5f,0.5f };
	playStart.size = { 500,125 };
	playStart.position = { 640,360,0 };
	playStart.GenerateSprite("playstart.png");

	for (int i = 0; i < 5; i++)
	{
		bossHp[i].anchorpoint = { 0.5f,0 };
		bossHp[i].GenerateSprite("Enemy_HP.png");
		bossHp[i].size = { 40,60 };
		bossHp[i].position = { i * 30.0f + 580.0f,30.0f,0.0f };
		bossHp[i].SpriteTransferVertexBuffer(false);
	}

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
	titleButton.size = { 315,50 };
	titleButton.GenerateSprite("toTitle.png");
	titleButton.position = { 640,500,0 };
	titleButton.SpriteTransferVertexBuffer();
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
	player.init(input, directx);

	skySphere = new Object3d_FBX;
	skySphere->Initialize();
	skySphere->SetModel(SkyModel);
	skySphere->SetScale({ 5.0f,5.0f,5.0f });

	cameraobj = new Object3d_FBX;
	cameraobj->Initialize();
	cameraobj->SetModel(model);

	srand(time(NULL));

	for (int i = 0; i < maxEnemyNum; i++)
	{
		testEnemys[i].init(enemyPattern::shot);
		testEnemys[i].set({
			(float)(rand() % 50 - 25),
			(float)(rand() % 30 + 15),
			(float)(rand() % 50 - 25) });
	}

	//ボスの初期化
	testBoss.bossInit();
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

	startButton.SpriteUpdate();
	resultScreen[0].SpriteUpdate();
	gameTitle.SpriteUpdate();

	if (isPushStart && !startButtonEase_y.getIsActive())
	{
		stage1.position.x = 640;
		isMoveStageIcon = false;
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

	if (!isMoveStageIcon)
	{
		float iconPos = stage1.position.x;

		//次のステージ
		if (input->Triger(DIK_RIGHT) && stageNum < 2)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Cubic, 20, iconPos, iconPos - 300);
			stageNum++;
			isMoveStageIcon = true;
		}
		//前のステージ
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
			testEnemys[i].reSet();
		}

		player.reset();

		testBoss.bossSet({ 0,5,0 });
		testBoss.bossReSet();
		isBoss = false;

		if (stageNum == 1)
		{
			for (int i = 0; i < maxEnemyNum; i++)
			{
				testEnemys[i].changePattern(enemyPattern::chase);
			}
			testBoss.changePattern(enemyPattern::chase);
		}
		else
		{
			for (int i = 0; i < maxEnemyNum; i++)
			{
				testEnemys[i].changePattern(enemyPattern::shot);
			}
			testBoss.changePattern(enemyPattern::shot);
		}

		countDownEase.set(easingType::easeOut, easingPattern::Quintic, 120, 450, 20);
		countDown[0].rotation = 0;
		countDown[1].rotation = 0;
		countDown[2].rotation = 0;
		countDownNum = 0;
		isCountDown = true;
		isStartIcon = false;

		isMoveScreen = false;
		isScreenEase = false;
		isTextEase = false;
		isPushTitle = false;
		titleButton.size = { 315,50 };

		player.update();

		scene = sceneType::play;
	}
}

//プレイ画面更新
void GameScene::Play_updata()
{
	if (scene != sceneType::play)
	{
		return;
	}

	if (isCountDown)
	{
		countDown[countDownNum].size = { countDownEase.easing(),countDownEase.easing() };
		countDown[countDownNum].rotation -= 4;
		countDown[countDownNum].SpriteTransferVertexBuffer();
		countDown[countDownNum].SpriteUpdate();

		if (!countDownEase.getIsActive())
		{
			if (countDownNum + 1 < 3)
			{
				countDownEase.set(easingType::easeOut, easingPattern::Quintic, 120, 450, 20);
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
		player.isStop = true;

		for (int i = 0; i < maxEnemyNum; i++)
		{
			testEnemys[i].isStop = true;
		}

		testBoss.isStop = true;
	}
	else
	{
		player.isStop = false;

		for (int i = 0; i < maxEnemyNum; i++)
		{
			testEnemys[i].isStop = false;
		}

		testBoss.isStop = false;
	}

	//プレイヤー更新
	player.update();

	//リセット
	if (input->push(DIK_R))
	{
		/*targetnum = 0;

		for (int i = 0; i < maxEnemyNum; i++)
		{
			testEnemys[i].reSet();
		}

		for (auto itre = Enemys.begin(); itre != Enemys.end(); itre++)
		{
			itre->reSet();
		}

		testBoss.bossReSet();*/
	}

	//敵(テスト)更新
	for (int i = 0; i < maxEnemyNum; i++)
	{
		testEnemys[i].update(player.playerObject->getPosition());
		player.checkPlayerEnemy(&testEnemys[i]);
		player.checkPlayerEnemyBullet(&testEnemys[i]);
	}

	//ボス更新
	testBoss.bossUpdate(&player);

	//ホーミング弾発射
	if (input->Mouse_LeftRelease() && !isCountDown)
	{
		for (int i = 0; i < targetnum; i++)
		{
			for (int j = 0; j < maxEnemyNum; j++)
			{
				if (testEnemys[j].isTargetSet && !testEnemys[j].isSetMissile)
				{
					player.playerMissiale[i].setPenemy(&testEnemys[j]);
					player.playerMissiale[i].start(player.playerObject->getPosition());
					testEnemys[j].isSetMissile = true;
					break;
				}
			}
		}

		if (testBoss.isTargetSet && !testBoss.isSetMissile)
		{
			for (int i = 0; i < MaxPlayerMissileNum; i++)
			{
				if (player.playerMissiale[i].isArive = false)
				{

				}
			}
			player.playerMissiale[0].setPenemy(&testBoss);
			player.playerMissiale[0].start(player.playerObject->getPosition());
			testBoss.isSetMissile = true;
		}

		targetnum = 0;
	}

	skySphere->Update();

	//マウスカーソル非表示
	ShowCursor(false);

	//敵がロックオンされているかどうか
	checkHitPlayerTarget();

	//プレイヤーの通常弾当たり判定
	for (int j = 0; j < MaxPlayerBulletNum; j++)
	{
		for (int i = 0; i < maxEnemyNum; i++)
		{
			player.playerBullet[j].checkHitEnemy(&testEnemys[i]);
			player.playerBullet[j].checkHitEnemyBullet(&testEnemys[i]);
		}
	}

	//通常弾とボスの当たり判定
	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		player.playerBullet[i].checkHitEnemy(&testBoss);
		player.playerBullet[i].checkHitEnemyBullet(&testBoss);
	}

	//死んでいる雑魚敵をカウント
	int count = 0;
	for (int i = 0; i < maxEnemyNum; i++)
	{
		if (!testEnemys[i].isDraw)
		{
			count++;
		}
	}

	//すべての雑魚敵が死んでいたらボス出現
	if (count >= maxEnemyNum && !isBoss)
	{
		testBoss.bossSet({ 0,5,0 });
		isBoss = true;
	}

	if (isBoss)
	{
		for (int i = 0; i < 5; i++)
		{
			bossHp[i].SpriteUpdate();
		}
	}

	//ボスを倒したorプレイヤーが死んだらリザルト
	if ((isBoss && !testBoss.isDraw))
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
	}

	if (player.playerHP <= 0)
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

//リザルト画面更新
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

#pragma endregion 各シーン更新

#pragma region 各シーン描画

//タイトル画面描画
void GameScene::Title_draw()
{
	if (scene != sceneType::title)
	{
		return;
	}
}

//レクと画面描画
void GameScene::Select_draw()
{
	if (scene != sceneType::select)
	{
		return;
	}
}

//プレイ画面描画
void GameScene::Play_draw()
{
	if (scene != sceneType::play)
	{
		return;
	}

	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	player.draw3D(directx);

	for (int i = 0; i < maxEnemyNum; i++)
	{
		testEnemys[i].draw3D(directx);
	}

	//ボス描画
	testBoss.draw3D(directx);
}

//リザルト画面描画
void GameScene::Result_draw()
{
	if (scene != sceneType::clear && scene != sceneType::over)
	{
		return;
	}

	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	player.draw3D(directx);

	for (int i = 0; i < maxEnemyNum; i++)
	{
		testEnemys[i].draw3D(directx);
	}

	//ボス描画
	testBoss.draw3D(directx);
}

#pragma endregion 各シーン描画

//更新
void GameScene::Updata()
{
	//マウス座標更新
	MOUSE_POS = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };

	//ゲーム時間カウント
	game_time++;

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

	sample_back.SpriteUpdate();

	//camera->Updata();

	debugs_print();
}

//背景スプライト描画
void GameScene::DrawBack()
{
	sample_back.DrawSprite(directx->cmdList.Get());
}

//描画
void GameScene::Draw3D()
{
	//ゲーム内シーンごとの描画
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
		for (int i = 0; i < maxEnemyNum; i++)
		{
			testEnemys[i].draw2D(directx);
		}

		//ボス描画(2d)
		testBoss.draw2D(directx);

		if (isBoss)
		{
			for (int i = 0; i < testBoss.HP; i++)
			{
				bossHp[i].DrawSprite(directx->cmdList.Get());
			}
		}

		player.draw2D(directx);

		if (isCountDown)
		{
			countDown[countDownNum].DrawSprite(directx->cmdList.Get());
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
	debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::checkHitPlayerTarget()
{
	if (!player.isRockOn)
	{
		return;
	}

	if (!input->Mouse_LeftPush())
	{
		return;
	}

	//通常の敵
	for (int i = 0; i < maxEnemyNum; i++)
	{
		XMFLOAT2 screenPos = testEnemys[i].enemyObject->worldToScleen();

		float dis = sqrtf(powf(player.targetFirst.position.x - screenPos.x, 2) + powf(player.targetFirst.position.y - screenPos.y, 2));

		if (dis < 56.5685f && !testEnemys[i].isTargetSet && targetnum < MaxPlayerMissileNum && testEnemys[i].Isarive)
		{
			testEnemys[i].isTargetSet = true;
			targetnum++;
		}
	}

	//ボス
	XMFLOAT2 screenPos = testBoss.enemyObject->worldToScleen();

	float dis = sqrtf(powf(player.targetFirst.position.x - screenPos.x, 2) + powf(player.targetFirst.position.y - screenPos.y, 2));

	if (dis < 56.5685f && !testBoss.isTargetSet && targetnum < MaxPlayerMissileNum && testBoss.isDraw)
	{
		testBoss.isTargetSet = true;
		targetnum++;
	}
}