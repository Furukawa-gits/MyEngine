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

	camera = new Camera();

	camera->SetTarget({ 0, 2.5f, 0 });
	camera->SetEye({ -10,5,0 });

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
	//object->PlayAnimation();


	followcamera = new FollowCamera();

	

	SkySphere = new Object3d_FBX;
	SkySphere->Initialize();
	SkySphere->SetModel(SkyModel);
	SkySphere->SetScale({ 5.0f,5.0f,5.0f });
}

//デバッグテキスト
void GameScene::debugs_print()
{
}

//タイトル画面更新
void GameScene::Title_update()
{
	object->Update();
	SkySphere->Update();
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
	SkySphere->Draw(directx->cmdList.Get());
	object->Draw(directx->cmdList.Get());
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
