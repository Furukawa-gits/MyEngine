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

	BallSprite1.anchorpoint = { 0.5f,0.5f };
	BallSprite1.position = { 100,160,0 };
	BallSprite1.size = { 50,50 };
	BallSprite1.GenerateSprite("Ball.png");

	BallSprite2.anchorpoint = { 0.5f,0.5f };
	BallSprite2.position = { 100,360,0 };
	BallSprite2.size = { 50,50 };
	BallSprite2.GenerateSprite("Ball.png");

	BallSprite3.anchorpoint = { 0.5f,0.5f };
	BallSprite3.position = { 100,560,0 };
	BallSprite3.size = { 50,50 };
	BallSprite3.GenerateSprite("Ball.png");
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

}

//デバッグテキスト
void GameScene::debugs_print()
{
	debugtext.Print("SPACE:Start", 50, 50);
	debugtext.Print("R:Reset", 50, 70);

	char text1[30];
	//snprintf(text1, sizeof(text1), "ball:Accel=%f", ball.Accel.x);
	debugtext.Print(text1, 50, 90);
	debugtext.Print("ball:M=1.0", 50, 110);
	//snprintf(text1, sizeof(text1), "ball:Repulsion=%f", ball.repulsion);
	debugtext.Print(text1, 50, 130);
}

//タイトル画面更新
void GameScene::Title_update()
{
	//リセット
	if (input->Triger(DIK_R))
	{
		in.isStart = false;
		out.isStart = false;
		inOut.isStart = false;
	}

	//スタート
	if (input->Triger(DIK_SPACE))
	{
		in.easingSet();
		out.easingSet();
		inOut.easingSet();
	}

	//各スプライトのX座標をイージング
	BallSprite1.position.x = in.easeInQuad(100, 900, 60);
	BallSprite2.position.x = out.easeOutQuad(100, 900, 60);
	BallSprite3.position.x = inOut.easeInOutQuad(100, 900, 60);

	BallSprite1.SpriteUpdate();
	BallSprite2.SpriteUpdate();
	BallSprite3.SpriteUpdate();
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

	//イージング用フレームカウント更新
	easing::easingUpdate();

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
	BallSprite1.DrawSprite(directx->cmdList.Get());
	BallSprite2.DrawSprite(directx->cmdList.Get());
	BallSprite3.DrawSprite(directx->cmdList.Get());
}
