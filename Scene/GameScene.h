#pragma once
#include"../Base/Window_Generate.h"
#include"../Base/DirectX_Base.h"
#include"../Base/TexManager.h"
#include"../2D/SpriteCommon.h"
#include"../2D/SpriteSingleunit.h"
#include"../Input/dxInput.h"
#include"../3D/3Dobject.h"
#include"../Audio/Audio.h"
#include"../2D/Debug_Text.h"
#include"../camera/Camera.h"
#include"../Player/Player.h"

enum scene
{
	title = 0,
	play = 1,
	over = 2,
	clear = 3
};

enum ChangePat
{
	X,
	Y
};

enum class wave
{
	first = 0,
	second = 1,
	third = 2
};

//ウェーブごとの敵の数
const int ENEMY_NUM_1 = 8;

const int ENEMY_NUM_2 = 8;

const int ENEMY_NUM_3 = 20;

class GameScene
{
public:
	//コンストラクタ
	GameScene();

	//デストラクタ
	~GameScene();

	//テクスチャ読み込みだけ関数
	void Load_textures();

	//音読み込みだけ関数
	void Load_sounds();

	//スプライト(各クラスに依存しないやつ)初期化
	void Load_Sprites();

	//初期化
	void Init(directX* directx, dxinput* input, Audio* audio);

	//デバッグテキスト更新
	void debugs_print();

	//タイトル画面アニメーション
	void Title_update();

	//プレイ画面更新
	void Play_update();

	//リザルト画面更新
	void Result_update();

	//シーンごとの描画
	void Title_draw();
	void Play_draw();
	void Result_draw();

	//イーズイン
	float easeIn(const float startX, const float endX, const float time)
	{
		float y = time * time;
		return startX * (1.0f - y) + endX * y;
	}

	//イーズアウト
	float easeOut(const float startX, const float endX, const float time)
	{
		float y = time * (2 - time);
		return startX * (1.0f - y) + endX * y;
	}

	//更新
	void Update();

	//描画
	void Draw();

public:

	static const int debugTextnum = 0;
	//基幹部分
	directX* directx = nullptr;
	dxinput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugtext;
	XMFLOAT3 MOUSE_POS;
	Camera* camera = nullptr;

	/// <summary>
	/// ゲームに使う変数等はここに
	/// </summary>
	SpriteCommon spritecommon;				//スプライト共通データ
	ObjectCommon object3dcommon;	//3dオブジェクト共通データ
	TexManager texture;						//テクスチャ

	//音
	SoundData Title_bgm;

	//スプライト(各クラスに依存しないやつ)
	SingleSprite title_sp;
	SingleSprite title_button;
	SingleSprite nextbutton;
	SingleSprite closebutton;
	SingleSprite clear_sp;
	SingleSprite gameover_sp;

	SingleSprite move_ui;
	SingleSprite shot_ui;
	SingleSprite missile_ui;

	//3dオブジェクト
	Player player;

	SingleSprite sample_back;

#pragma region 敵の初期位置インデックス
	XMFLOAT3 enemy_pos_First_Second[8] = {
		{ -50.0f, 40.0f,400.0f },
		{ -25.0f, 40.0f,400.0f },
		{  25.0f, 40.0f,400.0f },
		{  50.0f, 40.0f,400.0f },
		{ -50.0f,-40.0f,400.0f },
		{ -25.0f,-40.0f,400.0f },
		{  25.0f,-40.0f,400.0f },
		{  50.0f,-40.0f,400.0f }
	};

	XMFLOAT3 enemy_pos_Third[20] =
	{
		{-100, 70,400},
		{ -80, 60,400},
		{ -60, 50,400},
		{ -40, 40,400},
		{ -20, 30,400},

		{ 100, 70,400},
		{  80, 60,400},
		{  60, 50,400},
		{  40, 40,400},
		{  20, 30,400},

		{-100,-70,400},
		{ -80,-60,400},
		{ -60,-50,400},
		{ -40,-40,400},
		{ -20,-30,400},

		{ 100,-70,400},
		{  80,-60,400},
		{  60,-50,400},
		{  40,-40,400},
		{  20,-30,400}
	};
#pragma endregion

	Enemy enemy_1[ENEMY_NUM_1];

	Enemy enemy_2[ENEMY_NUM_2];

	Enemy enemy_3[ENEMY_NUM_3];

	//ゲーム時間
	int game_time = 0;

	//シーン
	//scene scene = scene::title;
	int scene = title;

	//ウェーブ
	//wave wave = wave::first;

	int wave = 1;

	bool Ischangescene = false;

	bool Isclose = false;

	//生き残っている敵の数
	int enemyarivenum = 0;


	//リザルトの表示切り替え
	int result_count = 0;

	//フェードのパターン
	int fade_patern;

	//シーンチェンジフラグ
	bool IsSceneChange = false;

	//シーンをまたぐフラグ
	bool Istitle_to_play = false;
	bool IsPlay_to_clear = false;
	bool IsPlay_to_over = false;
	bool IsClear_to_title = false;
	bool IsOver_to_title = false;

	//ウェーブチェンジフラグ
	bool IsWaveChange = false;

	//ガイド表示フラグ
	bool IsGuide = false;

	//シーン遷移用(イーズインアウト)
	float start_X = -1280.0f;
	float end_X = 0.0f;
	float end2_X = 1280.0f;

	float start_Y = -720.0f;
	float end_Y = 0.0f;
	float end2_Y = 720.0f;

	float maxTime = 1.0f;
	float timeRate = 0.0f;
	float addTime = 0.1f;

	float timeRate2 = 0.0f;
	float addTime2 = 0.1f;

	bool Isout = false;
	bool Isin = false;
};