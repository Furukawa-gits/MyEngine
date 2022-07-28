#include"GameScene.h"
#include"../FbxLoder/Object3d_FBX.h"
#include<cassert>

GameScene::GameScene()
{
}

GameScene::~GameScene()
{

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

	BallSprite.anchorpoint = { 0.5f,0.5f };
	BallSprite.size = { 50,50 };
	BallSprite.GenerateSprite("Ball.png");

	BallSprite2.anchorpoint = { 0.5f,0.5f };
	BallSprite2.position = { 640,360,0 };
	BallSprite2.size = { 10,10 };
	BallSprite2.GenerateSprite("Ball.png");
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


	ball.Pos = { 400,-150,0 };
	ball.Radius = { 25,25,25 };
	ball.IsSlide = true;

	//3dオブジェクト生成

}

//デバッグテキスト
void GameScene::debugs_print()
{
	debugtext.Print("SPACE:Start", 50, 50);
	debugtext.Print("R:Reset", 50, 70);

	char text1[30];
	snprintf(text1, sizeof(text1), "ball:Accel=%f", ball.Accel.x);
	debugtext.Print(text1, 50, 90);
	debugtext.Print("ball:M=1.0", 50, 110);
	snprintf(text1, sizeof(text1), "ball:Repulsion=%f", ball.repulsion);
	debugtext.Print(text1, 50, 130);
}

//タイトル画面更新
void GameScene::Title_update()
{
	if (input->Triger(DIK_R))
	{
		ball.Pos = { 400,-150,0 };
		ball.IsMove = false;
		ball.repulsion = 1.0f;
	}

	if (input->Triger(DIK_SPACE))
	{
		ball.Set({ 400,-150,0 }, { -15,-15,0 });
	}

	ball.circularMotion2D({ 640,-360,0 }, 3.0f);

	BallSprite.position = { ball.Pos.x,ball.Pos.y * -1,ball.Pos.z };
	BallSprite.SpriteUpdate();

	BallSprite2.SpriteUpdate();
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

	debugs_print();
}

void GameScene::DrawBack()
{
	//背景スプライト描画
	//sample_back.DrawSprite(directx->cmdList.Get());
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

	//debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::DrawSP()
{
	BallSprite.DrawSprite(directx->cmdList.Get());
	BallSprite2.DrawSprite(directx->cmdList.Get());
}
