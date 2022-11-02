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

//スプライト(各クラスに依存しないやつ)初期化
void GameScene::Load_Sprites()
{
	sample_back.size = { 1280,720 };
	sample_back.GenerateSprite("sample_back.jpg");
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
	testPlayer.init(input, directx);

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
	debugtext.Print("R : Reset", 10, 130, 1.0f);

	if (scene == sceneType::title)
	{
		debugtext.Print("Title", 10, 160, 1.0f);
		debugtext.Print("SPACE : start", 10, 190, 1.0f);
	}
	else if (scene == sceneType::play)
	{
		debugtext.Print("Play", 10, 160, 1.0f);
	}
	else
	{
		debugtext.Print("Result", 10, 160, 1.0f);
	}
}

#pragma region 各シーン更新

//タイトル画面更新
void GameScene::Title_update()
{
	if (scene != sceneType::title)
	{
		return;
	}

	if (input->Triger(DIK_SPACE))
	{
		up = 0;
		right = 0;
		objectRot = { 0,0,0 };

		pitch = 0.0f;
		yow = 0.0f;

		targetnum = 0;

		for (int i = 0; i < maxEnemyNum; i++)
		{
			testEnemys[i].reSet();
		}

		testPlayer.reset();

		testBoss.bossReSet();
		testBoss.HP = 30;
		isBoss = false;

		scene = sceneType::play;
	}
}

//プレイ画面更新
void GameScene::Play_update()
{
	if (scene != sceneType::play)
	{
		return;
	}

	//プレイヤー更新
	testPlayer.update();

	//リセット
	if (input->push(DIK_R))
	{
		up = 0;
		right = 0;
		objectRot = { 0,0,0 };

		pitch = 0.0f;
		yow = 0.0f;

		targetnum = 0;

		for (int i = 0; i < maxEnemyNum; i++)
		{
			testEnemys[i].reSet();
		}

		testBoss.bossReSet();
	}

	skySphere->Update();

	//敵(テスト)更新
	for (int i = 0; i < maxEnemyNum; i++)
	{
		testEnemys[i].update(testPlayer.playerObject->getPosition());
		testPlayer.checkPlayerEnemy(&testEnemys[i]);
		testPlayer.checkPlayerEnemyBullet(&testEnemys[i]);
	}

	//ボス更新
	testBoss.bossUpdate(&testPlayer);

	//マウスカーソル非表示
	ShowCursor(false);

	//敵がロックオンされているかどうか
	checkHitPlayerTarget();

	//ホーミング弾発射
	if (input->Mouse_LeftRelease())
	{
		for (int i = 0; i < targetnum; i++)
		{
			for (int j = 0; j < maxEnemyNum; j++)
			{
				if (testEnemys[j].isTargetSet && !testEnemys[j].isSetMissile)
				{
					testPlayer.playerMissiale[i].setPenemy(&testEnemys[j]);
					testPlayer.playerMissiale[i].start(testPlayer.playerObject->getPosition());
					testEnemys[j].isSetMissile = true;
					break;
				}
			}

			if (testBoss.isTargetSet && !testBoss.isSetMissile)
			{
				testPlayer.playerMissiale[i].setPenemy(&testBoss);
				testPlayer.playerMissiale[i].start(testPlayer.playerObject->getPosition());
				testBoss.isSetMissile = true;
			}
		}

		targetnum = 0;
	}

	//プレイヤーの通常弾当たり判定
	for (int j = 0; j < MaxPlayerBulletNum; j++)
	{
		for (int i = 0; i < maxEnemyNum; i++)
		{
			testPlayer.playerBullet[j].checkHitEnemy(&testEnemys[i]);
			testPlayer.playerBullet[j].checkHitEnemyBullet(&testEnemys[i]);
		}
	}

	//通常弾とボスの当たり判定
	for (int i = 0; i < MaxPlayerBulletNum; i++)
	{
		testPlayer.playerBullet[i].checkHitEnemy(&testBoss);
		testPlayer.playerBullet[i].checkHitEnemyBullet(&testBoss);
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

	//ボスを倒したorプレイヤーが死んだらリザルト
	if ((isBoss && !testBoss.isDraw) || testPlayer.playerHP <= 0)
	{
		scene = sceneType::clear;
	}

	//デバッグ用シーン切り替え
	if (input->Triger(DIK_N))
	{
		scene = sceneType::clear;
	}
}

//リザルト画面更新
void GameScene::Result_update()
{
	if (input->Triger(DIK_SPACE))
	{
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

//プレイ画面描画
void GameScene::Play_draw()
{
	if (scene != sceneType::play)
	{
		return;
	}

	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	testPlayer.draw3D(directx);

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

}

#pragma endregion 各シーン描画

//更新
void GameScene::Update()
{
	//マウス座標更新
	MOUSE_POS = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };

	//ゲーム時間カウント
	game_time++;

	//シーン切り替え

	//タイトル画面
	Title_update();

	//プレイ画面
	Play_update();

	//クリア画面
	if (scene == sceneType::clear || scene == sceneType::over)
	{
		Result_update();
	}

	sample_back.SpriteUpdate();

	//camera->Update();

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

	Play_draw();

	if (scene == sceneType::clear || scene == sceneType::over)
	{
		Result_draw();
	}
}

void GameScene::Draw2D()
{
	if (scene == sceneType::play)
	{
		for (int i = 0; i < maxEnemyNum; i++)
		{
			testEnemys[i].draw2D(directx);
		}

		//ボス描画(2d)
		testBoss.draw2D(directx);

		testPlayer.draw2D(directx);
	}
	debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::checkHitPlayerTarget()
{
	if (!testPlayer.isRockOn)
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

		float dis = sqrtf(powf(testPlayer.targetFirst.position.x - screenPos.x, 2) + powf(testPlayer.targetFirst.position.y - screenPos.y, 2));

		if (dis < 56.5685f && !testEnemys[i].isTargetSet && targetnum < MaxPlayerMissileNum && testEnemys[i].Isarive)
		{
			testEnemys[i].isTargetSet = true;
			targetnum++;
		}
	}

	//ボス
	XMFLOAT2 screenPos = testBoss.enemyObject->worldToScleen();

	float dis = sqrtf(powf(testPlayer.targetFirst.position.x - screenPos.x, 2) + powf(testPlayer.targetFirst.position.y - screenPos.y, 2));

	if (dis < 56.5685f && !testBoss.isTargetSet && targetnum < MaxPlayerMissileNum && testBoss.Isarive)
	{
		testBoss.isTargetSet = true;
		targetnum++;
	}
}
