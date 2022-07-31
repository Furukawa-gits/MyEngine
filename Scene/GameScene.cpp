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

	strings::setInput(input);

	for (int i = 0; i < 5; i++)
	{
		objs[i].init();
	}

	objs[0].stat = 0;
	objs[0].pos = { 640,100 };

	for (int i = 1; i < 5; i++)
	{
		objs[i].stat = 1;
		objs[i].vel = { 0,0 };
		objs[i].pos = objs[i - 1].pos;
		objs[i].link0 = &objs[i - 1];
		objs[i - 1].link1 = &objs[i];
	}

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
	objs[0].grabbed = 1;
	for (int i = 0; i < 5; i++)
	{
		objs[i].update();
	}
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
	for (int i = 0; i < 5; i++)
	{
		objs[i].disp(directx->cmdList.Get());
	}
}
