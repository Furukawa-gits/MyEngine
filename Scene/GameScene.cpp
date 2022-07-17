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

	target.anchorpoint = { 0.5f,0.5f };
	//target.size = { 50.0f,50.0f };
	target.GenerateSprite("Target.png");
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

	camera = new Camera();

	camera->SetTarget({ 0, 2.5f, 0 });
	camera->SetEye({ 0,5,-50 });
	//スプライトクラス初期化
	SingleSprite::SetStaticData(directx->dev.Get());

	//デバッグテキスト初期化
	debugtext.Init();

	//スプライト生成
	Load_Sprites();

	//3dオブジェクト生成
	Object3d_FBX::SetDevice(directx->dev.Get());

	Object3d_FBX::SetCamera(camera);

	Object3d_FBX::CreateGraphicsPipeline();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");
	SkyModel = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");

	object = new Object3d_FBX;
	object->Initialize();
	object->SetModel(model);
	object->SetPosition({ 0,15,0 });
	object->setSpeed(2.0f);
	object->PlayAnimation();

	followcamera = new FollowCamera();

	followcamera->setFollowTarget(&object->getPosition(), &object->getRotation(), -30);

	Object3d_FBX::SetCamera(followcamera);

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
			(float)(rand() % 30 - 15),
			(float)(rand() % 30),
			(float)(rand() % 30 - 15)
			});
	}
}

//デバッグテキスト
void GameScene::debugs_print()
{
}

//タイトル画面更新
void GameScene::Title_update()
{
	if (input->mouse_p.x >= 1000)
	{
		objectRot.y += 0.7f;
	}

	if (input->mouse_p.x <= 280)
	{
		objectRot.y -= 0.7f;
	}

	//前に進む
	if (input->push(DIK_W))
	{
		object->addMoveFront();
	}
	//後に下がる
	if (input->push(DIK_S))
	{
		object->addMoveBack();
	}

	if (input->push(DIK_R))
	{
		up = 0;
		right = 0;
		objectRot = { 0,0,0 };

		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].reSet();
		}
	}

	object->SetRotation(objectRot);

	followcamera->TargetObjectPos = &object->getPosition();
	followcamera->TargetObjectAngle = &object->getRotation();

	followcamera->Following();

	object->Update();
	skySphere->Update();

	cameraobj->SetPosition(followcamera->GetEye());
	cameraobj->SetRotation(*followcamera->TargetObjectAngle);
	cameraobj->Update();

	//敵(テスト)初期化
	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].update({ 0,0,0 });
	}

	//マウスカーソル非表示
	ShowCursor(false);
}

//プレイ画面更新
void GameScene::Play_update()
{

}

//リザルト画面更新
void GameScene::Result_update()
{

}

//タイトル画面描画
void GameScene::Title_draw()
{
	skySphere->Draw(directx->cmdList.Get());

	//directx->depthclear();
	object->Draw(directx->cmdList.Get());

	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].draw3D(directx);
	}

	//cameraobj->Draw(directx->cmdList.Get());
}

//プレイ画面描画
void GameScene::Play_draw()
{

}

//リザルト画面描画
void GameScene::Result_draw()
{
}


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

	if (input->Mouse_LeftPush())
	{
		mousePressCount++;
	}
	else
	{
		mousePressCount = 0;
	}

	if (mousePressCount > 30)
	{
		target.rotation -= 5.0f;
		isTarget = true;
	}
	else
	{
		target.rotation += 3.0f;
		isTarget = false;
	}

	//敵がロックオンされているかどうか
	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].isHitTarget({ target.position.x,target.position.y }, isTarget);
	}

	target.position = MOUSE_POS;
	target.SpriteTransferVertexBuffer();
	target.SpriteUpdate();

	if (input->Mouse_LeftRelease())
	{
		for (int i = 0; i < enemynum; i++)
		{
			if (testEnemys[i].Istarget_set)
			{
				testEnemys[i].Isarive = false;
			}
		}
	}

	if (input->Mouse_LeftTriger())
	{
		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].isHitShot({ target.position.x,target.position.y });
		}
	}

	camera->Update();
}

void GameScene::DrawBack()
{
	//背景描画
	sample_back.DrawSprite(directx->cmdList.Get());

	//深度バッファクリア
	//directx->depthclear();
}

//描画
void GameScene::Draw3D()
{
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

	target.DrawSprite(directx->cmdList.Get());
}
