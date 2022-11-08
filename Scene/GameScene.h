#pragma once
#include"../Base/WindowGenerate.h"
#include"../Base/DirectX_Base.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../Audio/Audio.h"
#include"../2D/Debug_Text.h"
#include"../camera/Camera.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"

#include"../Player/Player.h"
#include"../Player/Enemy.h"
#include"../Player/Boss.h"

#include<random>
#include<time.h>

enum class sceneType
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
//const int ENEMY_NUM_1 = 8;
//
//const int ENEMY_NUM_2 = 8;
//
//const int ENEMY_NUM_3 = 20;

class GameScene
{
public:
	//コンストラクタ
	GameScene();

	//デストラクタ
	~GameScene();

	//音読み込みだけ関数
	void Load_sounds();

	//スプライト(各クラスに依存しないやつ)初期化
	void Load_Sprites();

	//初期化
	void Init(directX* directx, dxinput* input, Audio* audio);

	//デバッグテキスト更新
	void debugs_print();

	//シーンごとの更新
	void Title_update();
	void Play_update();
	void Result_update();

	//シーンごとの描画
	void Title_draw();
	void Play_draw();
	void Result_draw();

	//更新
	void Update();

	//描画
	void DrawBack();
	void Draw3D();
	void Draw2D();

	//ホーミングターゲットのセッティング
	void checkHitPlayerTarget();

	bool Isclose = false;

private:

	//-------------ゲームに使う変数等はここより↓-------------
	static const int debugTextnum = 0;

#pragma region 基幹部分
	directX* directx = nullptr;
	dxinput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugtext;
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	//Camera* camera = nullptr;
#pragma endregion

	//回転量
	XMFLOAT3 objectRot = { 0,0,0 };

	float up = 0.0f;
	float right = 0.0f;

	float pitch = 0.0f;
	float yow = 0.0f;
	float roll = 0.0f;
	//音

	//スプライト(各クラスに依存しないやつ)

	//3dオブジェクト
	Model* model = nullptr;
	Model* SkyModel = nullptr;
	Object3d_FBX* object = nullptr;
	Object3d_FBX* skySphere = nullptr;
	Object3d_FBX* cameraobj = nullptr;

	//プレイヤー
	Player testPlayer;

	int targetnum = 0;

	//敵(test)
	static const int maxEnemyNum = 20;
	Enemy testEnemys[maxEnemyNum];
	bool isEnemySimple = false;

	/// <summary>
	/// テスト用ボス
	/// <para>初期化処理内でHPを30に設定</para>
	/// <para>アップデート処理内で敵が全滅するまで待つ</para>
	/// <para>全滅したら出現</para>
	/// <para>HP削り切ったら消滅</para>
	/// オブジェクトのサイズは大きくして分かりやすく
	/// </summary>
	Boss testBoss;

	//ボス出現フラグ
	bool isBoss = false;

	//照準スプライト関連
	int mousePressCount = 0;
	SingleSprite target;
	bool isTarget = false;

	//背景
	SingleSprite sample_back;

	//ゲーム時間
	int game_time = 0;

	//シーン
	sceneType scene = sceneType::title;

	//ウェーブ
	bool Ischangescene = false;

	//リザルトの表示切り替え
	int result_count = 0;
};