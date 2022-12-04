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
#include"../Player/CheckHItManager.h"

#include"../staging/Easing.h"

#include<random>
#include<time.h>

#include<memory>
#include<list>

enum class sceneType
{
	title = 0,
	select = 1,
	play = 2,
	over = 3,
	clear = 4
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

class GameScene
{
public:
	//コンストラクタ
	GameScene();

	//デストラクタ
	~GameScene();

	//初期化
	void Init(directX* directx, dxinput* input, Audio* audio);

	//更新
	void Updata();

	//描画
	void DrawBack();
	void Draw3D();
	void Draw2D();

	bool Isclose = false;

private:

	//シーンごとの更新
	void Title_updata();
	void Select_updata();
	void Play_updata();
	void Result_updata();

	//シーンごとの描画
	void Title_draw();
	void Select_draw();
	void Play_draw();
	void Result_draw();

	//ホーミングターゲットのセッティング
	void checkHitPlayerTarget();

	//音読み込みだけ関数
	void Load_sounds();

	//スプライト(各クラスに依存しないやつ)初期化
	void Load_Sprites();

	//デバッグテキスト更新
	void debugs_print();

	//-------------ゲームに使う変数等はここより↓-------------
	static const int debugTextnum = 0;

#pragma region 基幹部分
	directX* directx = nullptr;
	dxinput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugtext;
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
#pragma endregion
	//音

	//3dオブジェクト
	Model* model = nullptr;
	Model* SkyModel = nullptr;
	Object3d_FBX* object = nullptr;
	Object3d_FBX* skySphere = nullptr;
	Object3d_FBX* cameraobj = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_p;

	int targetnum = 0;

	//敵(test)
	static const int maxEnemyNum = 20;
	std::list<std::unique_ptr<Enemy>> enemyList;
	bool isEnemySimple = false;

	/// <summary>
	/// テスト用ボス
	/// <para>初期化処理内でHPを30に設定</para>
	/// <para>アップデート処理内で敵が全滅するまで待つ</para>
	/// <para>全滅したら出現</para>
	/// <para>HP削り切ったら消滅</para>
	/// オブジェクトのサイズは大きくして分かりやすく
	/// </summary>
	std::unique_ptr<Boss> testBoss;

	//ボスのhpゲージ
	std::vector<std::unique_ptr<SingleSprite>> bossHitPoints;

	//ボス出現フラグ
	bool isBoss = false;

	//照準スプライト関連
	int mousePressCount = 0;
	SingleSprite target;
	bool isTarget = false;

	//スプライト(各クラスに依存しないやつ)
	//背景
	std::unique_ptr<SingleSprite> sample_back;

	//タイトル
	SingleSprite gameTitle;

	//スタートボタン
	SingleSprite startButton;

	//ステージアイコン
	SingleSprite stage1;
	SingleSprite stage2;

	SingleSprite selects[2];

	//カウントダウン・スタートアイコン
	SingleSprite countDown[3];
	SingleSprite playStart;

	//リザルト画面
	SingleSprite resultScreen[2];

	//クリア画面
	SingleSprite clearText;

	//ゲームオーバー画面
	SingleSprite overText;

	//タイトル・セレクトボタン
	SingleSprite titleButton;

	//ゲーム時間
	int game_time = 0;

	//シーン番号
	sceneType scene = sceneType::title;

	//タイトル演出フラグ
	bool isTitleStage = false;

	//スタートボタンのイージング
	easingManager startButtonEase_y;
	easingManager startButtonEase_x;

	bool isPushStart = false;

	//シーンチェンジフラグ
	bool Ischangescene = false;

	//ステージ番号
	int stageNum = 1;

	//敵を全滅させたときに上がるレベル
	int level = 1;

	//ステージごとのレベル
	int stageLevel = 1;

	// レベル = ステージレベル + 2
	// レベルが最大まで行くとボス出現
	// ボスの HP はレベル + 5
	// ボスを倒すとクリア

	//ステージアイコンのイージング
	easingManager stageIconEase;

	//ステージアイコンを動かしているかどうか
	bool isMoveStageIcon = false;

	//カウントダウンのイージングフラグ
	bool isCountDown = false;
	bool isStartIcon = false;
	int countDownNum = 0;
	int startIconTime = 0;
	int countDownTime = 150;
	easingManager countDownEase;
	easingManager startEase;

	//ボスのHPゲージ出現のイージング
	easingManager bossHpEase;
	bool isBossHp = false;

	//リザルト画面のイージングフラグ
	bool isMoveScreen = false;
	bool isScreenEase = false;
	bool isTextEase = false;

	easingManager resultScreenEase;
	easingManager clearTextEase;
	easingManager overTextEase;

	//タイトルボタンのイージング
	easingManager titleButtonEase_y;
	easingManager titleButtonEase_x;

	bool isPushTitle = false;
};