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

//テクスチャ読み込みだけ関数
void GameScene::Load_textures()
{
	texture.LoadTexture(1, L"Resources/Image/sample_back.jpg", directx->dev.Get());
	texture.LoadTexture(2, L"Resources/Ball.png", directx->dev.Get());
}

//サウンドだけ読み込み関数
void GameScene::Load_sounds()
{

}

//スプライト(各クラスに依存しないやつ)初期化
void GameScene::Load_Sprites()
{
	sample_back.size = { 1280,720 };
	sample_back.texSize = { 1280,720 };
	sample_back.GenerateSprite(directx->dev.Get(), win_width, win_hight, 1, &texture);

	BallSprite.size = { 200,200 };
	BallSprite.GenerateSprite(directx->dev.Get(), win_width, win_hight, 2, &texture);
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

	Load_sounds();

	//3dオブジェクト共通データ生成
	object3dcommon.init(directx);

	camera = new Camera(1280, 720);

	camera->SetTarget({ 0, 2.5f, 0 });
	camera->SetEye({ -10,5,0 });

	//テクスチャ初期化
	texture.Init(directx->dev.Get());

	//デバッグテキストテクスチャ読み込み
	texture.LoadTexture(debugTextnum, L"Resources/Image/Debug_Text.png", directx->dev.Get());

	//デバッグテキスト初期化
	debugtext.Init(directx->dev.Get(), win_width, win_hight, debugTextnum, &texture);

	//テクスチャ読み込み
	Load_textures();

	//スプライト共通データ生成
	spritecommon.SpriteCommonCreate(directx->dev.Get(), win_width, win_hight);

	//スプライト初期化
	Load_Sprites();


	ball.Pos = { 100,-400,0 };
	ball.IsThrow = true;
	BallSprite.SpriteTransferVertexBuffer(&texture, false);

	//3dオブジェクト生成
	Object3d_FBX::SetDevice(directx->dev.Get());

	Object3d_FBX::SetCamera(camera);

	Object3d_FBX::CreateGraphicsPipeline();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");

	object = new Object3d_FBX;
	object->Initialize();
	object->SetModel(model);
	object->PlayAnimation();

}

//デバッグテキスト
void GameScene::debugs_print()
{
	char text[20] = "SPACE:Start";
	char text_1[20] = "R:Reset";
	debugtext.Print(spritecommon, &texture, text, 50, 50);
	debugtext.Print(spritecommon, &texture, text_1, 50, 70);
}

//タイトル画面更新
void GameScene::Title_update()
{

	if (input->Triger(DIK_R))
	{
		ball.Pos = { 100,-400,0 };
		ball.IsMove = false;
	}

	if (input->Triger(DIK_SPACE))
	{
		ball.Set({ 100,-400,0 }, { 35,30,0 });
	}

	ball.Update(0.8f);

	BallSprite.position = { ball.Pos.x,ball.Pos.y * -1,ball.Pos.z };
	BallSprite.SpriteUpdate(spritecommon);

	debugs_print();

	object->Update();
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
	BallSprite.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());

	//object->Draw(directx->cmdList.Get());

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

	sample_back.SpriteUpdate(spritecommon);

	camera->Update();
}

//描画
void GameScene::Draw()
{
	//背景描画
	spritecommon.SpriteCommonBeginDraw(directx->cmdList.Get(), &texture);
	sample_back.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());

	//深度バッファクリア
	directx->depthclear();

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

	debugtext.DrawAll(directx->cmdList.Get(), spritecommon, &texture, directx->dev.Get());
}
