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

	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].init(i, i);
		testEnemys[i].set({
			(float)(rand() % 50 - 25),
			(float)(rand() % 30),
			(float)(rand() % 50 - 25)
			});
	}

	testBoss.init(0, 0);
	testBoss.HP = 30;
}

//デバッグテキスト
void GameScene::debugs_print()
{
	debugtext.Print("W : Move Front", 10, 10, 1.0f);
	debugtext.Print("S : Move Back", 10, 25, 1.0f);
	debugtext.Print("MouseDrag : Camera(Left&Right)", 10, 55, 1.0f);
	debugtext.Print("MouseClick : Shot", 10, 70, 1.0f);
	debugtext.Print("MousePress(Left)&Drag : Target", 10, 85, 1.0f);
	debugtext.Print("MouseRelease : Homing", 10, 100, 1.0f);
	debugtext.Print("R : Reset", 10, 130, 1.0f);

	if (scene == title)
	{
		debugtext.Print("Title", 10, 160, 1.0f);
		debugtext.Print("SPACE : start", 10, 190, 1.0f);
	}
	else if (scene == play)
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
	if (input->Triger(DIK_SPACE))
	{
		up = 0;
		right = 0;
		objectRot = { 0,0,0 };

		pitch = 0.0f;
		yow = 0.0f;

		targetnum = 0;

		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].reSet();
		}

		testPlayer.reset();

		testBoss.reSet();
		testBoss.Isarive = false;
		testBoss.HP = 30;

		scene = play;
	}
}

//プレイ画面更新
void GameScene::Play_update()
{
	//プレイヤー更新
	testPlayer.update();

	if (input->push(DIK_R))
	{
		up = 0;
		right = 0;
		objectRot = { 0,0,0 };

		pitch = 0.0f;
		yow = 0.0f;

		targetnum = 0;

		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].reSet();
		}

		testPlayer.reset();

		testBoss.reSet();
		testBoss.Isarive = false;
		testBoss.HP = 30;
	}

	skySphere->Update();

	//敵(テスト)更新
	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].update(testPlayer.Player_object->getPosition());
		testPlayer.checkPlayerEnemy(&testEnemys[i]);
	}

	//マウスカーソル非表示
	ShowCursor(false);

	//敵がロックオンされているかどうか
	checkHitPlayerTarget();

	if (input->Mouse_LeftRelease())
	{
		for (int i = 0; i < targetnum; i++)
		{
			for (int j = 0; j < enemynum; j++)
			{
				if (testEnemys[j].Istarget_set && !testEnemys[j].IsSetMissile)
				{
					testPlayer.player_missiale[i].setPenemy(&testEnemys[j]);
					testPlayer.player_missiale[i].start(testPlayer.Player_object->getPosition());
					testEnemys[j].IsSetMissile = true;
					break;
				}
			}
		}

		targetnum = 0;
	}


	for (int j = 0; j < MaxPlayerBulletNum; j++)
	{
		for (int i = 0; i < enemynum; i++)
		{
			testPlayer.player_bullet[j].checkhit(&testEnemys[i]);
		}
	}

	int count = 0;

	for (int i = 0; i < enemynum; i++)
	{
		if (!testEnemys[i].Isarive)
		{
			count++;
		}
	}

	if (count >= enemynum || testPlayer.HP <= 0)
	{
		scene = clear;
	}
}

//リザルト画面更新
void GameScene::Result_update()
{
	if (input->Triger(DIK_SPACE))
	{
		scene = title;
	}
}

#pragma endregion 各シーン更新

#pragma region 各シーン描画

//タイトル画面描画
void GameScene::Title_draw()
{

}

//プレイ画面描画
void GameScene::Play_draw()
{
	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	testPlayer.draw_3d(directx);

	for (int i = 0; i < enemynum; i++)
	{
		//testEnemys[i].draw3D(directx);
	}
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
	if (scene == title)
	{
		Title_update();
	}

	//プレイ画面
	if (scene == play)
	{
		Play_update();
	}

	//クリア画面
	if (scene == clear || scene == over)
	{
		Result_update();
	}

	sample_back.SpriteUpdate();

	//camera->Update();

	debugs_print();
}

void GameScene::DrawBack()
{
	//背景スプライト描画
	sample_back.DrawSprite(directx->cmdList.Get());
}

//描画
void GameScene::Draw3D()
{
	//ゲーム内シーンごとの描画
	if (scene == title)
	{
		Title_draw();
	}
	else if (scene == play)
	{
		Play_draw();
	}
	else
	{
		Result_draw();
	}
}

void GameScene::DrawSP()
{
	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].drawSp(directx);
	}

	if (scene == play)
	{
		testPlayer.draw_2d(directx);
	}
	debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::checkHitPlayerTarget()
{
	if (!testPlayer.Isrockon)
	{
		return;
	}

	if (!input->Mouse_LeftPush())
	{
		return;
	}

	for (int i = 0; i < enemynum; i++)
	{
		XMFLOAT2 screenPos = testEnemys[i].testObject->worldToScleen();

		float dis = sqrtf(powf(input->mouse_position.x - screenPos.x, 2) + powf(input->mouse_position.y - screenPos.y, 2));

		if (dis < 20 && !testEnemys[i].Istarget_set && targetnum < MaxPlayerMissileNum)
		{
			testEnemys[i].Istarget_set = true;
			targetnum++;
		}

		testEnemys[i].Rock_Target.position = { screenPos.x,screenPos.y,0 };
	}
}
