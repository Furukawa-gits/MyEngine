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

	//スプライト(各クラスに依存しないやつ)

	//3dオブジェクト

	SingleSprite sample_back;

	//ゲーム時間
	int game_time = 0;

	//シーン
	//scene scene = scene::title;
	int scene = title;

	//ウェーブ
	bool Ischangescene = false;

	bool Isclose = false;

	//リザルトの表示切り替え
	int result_count = 0;
};