#include"GameScene.h"
#include<cassert>

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

//テクスチャ読み込みだけ関数
void GameScene::Load_textures()
{
	texture.LoadTexture(1, L"Resources/Image/sample_back.jpg", directx->dev.Get());
	texture.LoadTexture(2, L"Resources/Image/Target.png", directx->dev.Get());
	texture.LoadTexture(3, L"Resources/Image/Rock_on.png", directx->dev.Get());

	texture.LoadTexture(4, L"Resources/Image/Title.png", directx->dev.Get());
	texture.LoadTexture(5, L"Resources/Image/Start_button.png", directx->dev.Get());
	texture.LoadTexture(6, L"Resources/Image/Click_Next.png", directx->dev.Get());
	texture.LoadTexture(7, L"Resources/Image/Click_Title.png", directx->dev.Get());
	texture.LoadTexture(8, L"Resources/Image/Stage_Clear.png", directx->dev.Get());
	texture.LoadTexture(9, L"Resources/Image/Game_Over.png", directx->dev.Get());
	texture.LoadTexture(10, L"Resources/Image/Move_UI.png", directx->dev.Get());
	texture.LoadTexture(11, L"Resources/Image/Shot_UI.png", directx->dev.Get());
	texture.LoadTexture(12, L"Resources/Image/Missile_UI.png", directx->dev.Get());

	texture.LoadTexture(13, L"Resources/Image/Player_HP.png", directx->dev.Get());
}

//サウンドだけ読み込み関数
void GameScene::Load_sounds()
{
	Title_bgm = audio->LoadSoundWave("Resources/Sound's/titleBack.wav", Title_bgm);
}

//スプライト(各クラスに依存しないやつ)初期化
void GameScene::Load_Sprites()
{
	sample_back.texnumber = 1;
	sample_back.GenerateSprite(directx->dev.Get(), win_width, win_hight, sample_back.texnumber, &texture, false, false, false, false);
	sample_back.anchorpoint = { 0,0 };
	sample_back.size = { 1280,720 };
	sample_back.texLeftTop = { 0,0 };
	sample_back.texSize = { 1280,720 };
	sample_back.SpriteTransferVertexBuffer(&texture, false);


	title_sp.GenerateSprite(directx->dev.Get(), win_width, win_hight, 4, &texture, false, false, false, false);
	title_sp.position = { 640,270,0 };
	title_sp.size = { 512,256 };
	title_sp.SpriteTransferVertexBuffer(&texture, false);

	title_button.GenerateSprite(directx->dev.Get(), win_width, win_hight, 5, &texture, false, false, false, false);
	title_button.position = { 640,450,0 };
	title_button.size = { 256,64 };
	title_button.SpriteTransferVertexBuffer(&texture, false);

	nextbutton.GenerateSprite(directx->dev.Get(), win_width, win_hight, 6, &texture, false, false, false, false);
	nextbutton.position = { 640,450,0 };
	nextbutton.size = { 256,64 };
	nextbutton.SpriteTransferVertexBuffer(&texture, false);

	closebutton.GenerateSprite(directx->dev.Get(), win_width, win_hight, 7, &texture, false, false, false, false);
	closebutton.position = { 640,450,0 };
	closebutton.size = { 256,64 };
	closebutton.SpriteTransferVertexBuffer(&texture, false);

	clear_sp.GenerateSprite(directx->dev.Get(), win_width, win_hight, 8, &texture, false, false, false, false);
	clear_sp.position = { 640,270,0 };
	clear_sp.size = { 512,128 };
	clear_sp.SpriteTransferVertexBuffer(&texture, false);

	gameover_sp.GenerateSprite(directx->dev.Get(), win_width, win_hight, 9, &texture, false, false, false, false);
	gameover_sp.position = { 640,270,0 };
	gameover_sp.size = { 512,128 };
	gameover_sp.SpriteTransferVertexBuffer(&texture, false);

	shot_ui.GenerateSprite(directx->dev.Get(), win_width, win_hight, 11, &texture, false, false, false, false);
	shot_ui.anchorpoint = { 0.0f,0.0f };
	shot_ui.position = { 10.0f,10.0f,0.0f };
	shot_ui.size = { 200,50 };
	shot_ui.SpriteTransferVertexBuffer(&texture, false);

	missile_ui.GenerateSprite(directx->dev.Get(), win_width, win_hight, 12, &texture, false, false, false, false);
	missile_ui.anchorpoint = { 0.0f,0.0f };
	missile_ui.position = { 10.0f, shot_ui.position.y + 50.0f, 0.0f };
	missile_ui.size = { 300,50 };
	missile_ui.SpriteTransferVertexBuffer(&texture, false);

	move_ui.GenerateSprite(directx->dev.Get(), win_width, win_hight, 10, &texture, false, false, false, false);
	move_ui.anchorpoint = { 0,0 };
	move_ui.position = { 10.0f,missile_ui.position.y + 50.0f,0.0f };
	move_ui.size = { 250,50 };
	move_ui.SpriteTransferVertexBuffer(&texture, false);
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

	object3D::SetStaticData(camera, this->directx, &object3dcommon);

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

	//3dオブジェクト生成
	player.init(input, &texture, this->directx);

	//敵初期化
	for (int i = 0; i < ENEMY_NUM_1; i++)
	{
		enemy_1[i].init(i + 30, i + 40, &texture);
	}

	for (int i = 0; i < ENEMY_NUM_2; i++)
	{
		enemy_2[i].init(i + 50, i + 60, &texture);
	}

	for (int i = 0; i < ENEMY_NUM_3; i++)
	{
		enemy_3[i].init(i + 70, i + 90, &texture);
	}

	audio->PlaySoundWave_Val(Title_bgm, true, 0.7f);
}

//デバッグテキスト
void GameScene::debugs_print()
{
	char test_st_0[30];
	char test_st_1[30];
	char test_st_2[30];
	char test_st_4[30];
	char test_st_6[30];
	char test_st_7[30];

	//snprintf(test_st_0, sizeof(test_st_0), "score_rate : %f", Score_rate);
	//snprintf(test_st_1, sizeof(test_st_1), "player_score : %f", player.Score);

	//debugtext.Print(spritecommon, &texture, test_st_0, 800, 05);
	//debugtext.Print(spritecommon, &texture, test_st_1, 800, 25);

}

//タイトル画面更新
void GameScene::Title_update()
{
	if (input->Triger(DIK_SPACE))
	{
		Ischangescene = true;
	}

	if (Ischangescene)
	{
		for (int i = 0; i < ENEMY_NUM_1; i++)
		{
			enemy_1[i].setenemy(1, enemy_pos_First_Second[i]);
		}
		scene = play;
		wave = 1;
		Ischangescene = false;
	}
	title_sp.SpriteUpdate(spritecommon);
	title_button.SpriteUpdate(spritecommon);
}

//プレイ画面更新
void GameScene::Play_update()
{
	if (Ischangescene == false)
	{
		if (wave == 1)
		{
			player.update(&spritecommon);

			for (int i = 0; i < ENEMY_NUM_1; i++)
			{
				enemy_1[i].update(&spritecommon, player.Player_object.position);
				player.checkrockon(enemy_1[i]);
				player.checkplayerbullet(&enemy_1[i]);
				player.checkenemybullet(&enemy_1[i].bullet);
			}

			enemyarivenum = 0;
			for (int i = 0; i < ENEMY_NUM_1; i++)
			{
				if (enemy_1[i].Isarive == true)
				{
					enemyarivenum++;
				}
			}

			if (enemyarivenum <= 0)
			{
				nextbutton.SpriteUpdate(spritecommon);
				clear_sp.SpriteUpdate(spritecommon);

				if (input->Triger(DIK_SPACE))
				{
					player.HP = 10;
					for (int i = 0; i < ENEMY_NUM_2; i++)
					{
						enemy_2[i].setenemy(2, enemy_pos_First_Second[i]);
					}
					wave = 2;
				}
			}

			move_ui.SpriteUpdate(spritecommon);
			shot_ui.SpriteUpdate(spritecommon);
			missile_ui.SpriteUpdate(spritecommon);
		}
		else if (wave == 2)
		{
			player.update(&spritecommon);

			for (int i = 0; i < ENEMY_NUM_2; i++)
			{
				enemy_2[i].update(&spritecommon, player.Player_object.position);
				player.checkrockon(enemy_2[i]);
				player.checkplayerbullet(&enemy_2[i]);
				player.checkenemybullet(&enemy_2[i].bullet);
			}

			enemyarivenum = 0;
			for (int i = 0; i < ENEMY_NUM_2; i++)
			{
				if (enemy_2[i].Isarive == true)
				{
					enemyarivenum++;
				}
			}

			if (enemyarivenum <= 0)
			{
				nextbutton.SpriteUpdate(spritecommon);
				clear_sp.SpriteUpdate(spritecommon);

				if (input->Triger(DIK_SPACE))
				{
					player.HP = 10;
					for (int i = 0; i < ENEMY_NUM_3; i++)
					{
						enemy_3[i].setenemy(3, enemy_pos_Third[i]);
					}

					wave = 3;
				}
			}

			if (player.HP <= 0)
			{
				scene = over;
			}
		}
		else if (wave == 3)
		{
			player.update(&spritecommon);

			for (int i = 0; i < ENEMY_NUM_3; i++)
			{
				enemy_3[i].update(&spritecommon, player.Player_object.position);
				player.checkrockon(enemy_3[i]);
				player.checkplayerbullet(&enemy_3[i]);
				player.checkenemybullet(&enemy_3[i].bullet);
			}

			enemyarivenum = 0;
			for (int i = 0; i < ENEMY_NUM_3; i++)
			{
				if (enemy_3[i].Isarive == true)
				{
					enemyarivenum++;
				}
			}

			if (enemyarivenum <= 0)
			{
				Ischangescene = true;
			}

			if (player.HP <= 0)
			{
				scene = over;
			}
		}
	}

	if (Ischangescene)
	{
		scene = clear;
		Ischangescene = false;
	}
}

//リザルト画面更新
void GameScene::Result_update()
{
	if (scene == clear)
	{
		clear_sp.SpriteUpdate(spritecommon);
		closebutton.SpriteUpdate(spritecommon);
		if (input->Triger(DIK_SPACE))
		{
			Isclose = true;
		}
	}
	else if (scene == over)
	{
		gameover_sp.SpriteUpdate(spritecommon);
		closebutton.SpriteUpdate(spritecommon);
		if (input->Triger(DIK_SPACE))
		{
			Isclose = true;
		}
	}
}

void GameScene::Title_draw()
{
	spritecommon.SpriteCommonBeginDraw(directx->cmdList.Get(), &texture);
	title_sp.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
	title_button.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
}

void GameScene::Play_draw()
{
	if (wave == 1)
	{
		for (int i = 0; i < ENEMY_NUM_1; i++)
		{
			enemy_1[i].draw(directx, &texture, &spritecommon);
		}

		player.draw(directx, &texture, &spritecommon);

		directx->depthclear();
		spritecommon.SpriteCommonBeginDraw(directx->cmdList.Get(), &texture);
		move_ui.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
		shot_ui.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
		missile_ui.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());

		if (enemyarivenum <= 0)
		{
			nextbutton.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
			clear_sp.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
		}
	}
	else if (wave == 2)
	{
		for (int i = 0; i < ENEMY_NUM_2; i++)
		{
			enemy_2[i].draw(directx, &texture, &spritecommon);
		}

		player.draw(directx, &texture, &spritecommon);

		if (enemyarivenum <= 0)
		{
			nextbutton.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
			clear_sp.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
		}
	}
	else if (wave == 3)
	{
		for (int i = 0; i < ENEMY_NUM_3; i++)
		{
			enemy_3[i].draw(directx, &texture, &spritecommon);
		}

		player.draw(directx, &texture, &spritecommon);
	}
}

void GameScene::Result_draw()
{
	spritecommon.SpriteCommonBeginDraw(directx->cmdList.Get(), &texture);

	if (scene == clear)
	{
		clear_sp.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
		closebutton.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
	}
	else if (scene == over)
	{
		gameover_sp.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
		closebutton.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
	}
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
}
