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
#include"../Player/uniteBoss.h"
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
	void init(directX* directx, dxinput* input, Audio* audio);

	//更新
	void updata();

	//描画
	void drawBack();
	void draw3D();
	void draw2D();

	bool Isclose = false;

private:

	//シーンごとの更新
	void titleUpdata();
	void selectUpdata();
	void playUpdata();
	void resultUpdata();

	//チュートリアル
	void tutorial();

	//ステージ読み込み
	bool loadStage();

	//シーンごとの描画
	void titleDraw3d();
	void titleDraw2d();

	void selectDraw3d();
	void selectDraw2d();

	void playDraw3d();
	void playDraw2d();

	void resultDraw3d();
	void resultDraw2d();

	//ウェーブバー描画
	void drawNowWave();

	//ミニマップ描画
	void drawPositionUI();

	//ホーミングターゲットのセッティング
	void checkHitPlayerTarget();

	//カウントダウン
	void countDown();

	//音読み込みだけ関数
	void loadSounds();

	//スプライト(各クラスに依存しないやつ)初期化
	void loadSprites();

	//デバッグテキスト更新
	void debugsPrint();

	//当たり判定関数
	void checkHitEverything();

	//-------------ゲームに使う変数等はここより↓-------------
	static const int debugTextnum = 0;

#pragma region 基幹部分
	std::unique_ptr<directX> directx = nullptr;
	std::unique_ptr<dxinput> input = nullptr;
	std::unique_ptr<Audio> audio = nullptr;
	DebugText debugtext;
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
#pragma endregion
	//音

	//3dオブジェクト
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> SkyModel;
	std::unique_ptr<Model> groundModel;
	std::unique_ptr<object3dFBX> skySphere;
	std::unique_ptr<object3dFBX> groundPlane;

	//スカイドームの回転
	float skyShpereRotY = 0.0002f;

	//ライト
	Light* light = nullptr;

	Camera* stagingCamera = nullptr;

	//プレイヤー
	std::unique_ptr<Player> playerPointer;

	int targetnum = 0;

	//敵
	std::list<std::unique_ptr<Enemy>> enemyList;

	/// <summary>
	/// テスト用ボス
	/// <para>初期化処理内でHPを30に設定</para>
	/// <para>アップデート処理内で敵が全滅するまで待つ</para>
	/// <para>全滅したら出現</para>
	/// <para>HP削り切ったら消滅</para>
	/// オブジェクトのサイズは大きくして分かりやすく
	/// </summary>
	std::unique_ptr<Boss> testBoss;

	std::unique_ptr<uniteBoss> testUniteBoss;

	//ボスのhpゲージ
	std::vector<std::unique_ptr<SingleSprite>> bossHitPoints;

	//ボス出現フラグ
	bool isBoss = false;

	//スプライト(各クラスに依存しないやつ)
	//背景
	std::unique_ptr<SingleSprite> sample_back;
	//タイトル
	SingleSprite titleIconDrag;
	SingleSprite titleIconShoot;
	SingleSprite titlePlayer;
	SingleSprite titleWhiteBack;
	//スタートボタン
	SingleSprite startButton;
	//ステージアイコン
	SingleSprite stage[5];
	//セレクトアイコン
	SingleSprite selects[3];
	float selectIconSizeX = 150;
	//プレイスタートボタン
	SingleSprite spaceStart;
	//カウントダウン・スタートアイコン
	SingleSprite countDownSprite[3];
	SingleSprite playStart;
	//リザルト画面
	SingleSprite resultScreen[2];
	//クリア画面
	SingleSprite clearText;
	//ゲームオーバー画面
	SingleSprite overText;
	//タイトル・セレクトボタン
	SingleSprite titleButton;
	SingleSprite selectButton;
	//チュートリアル誘導
	SingleSprite toTutorial;
	//プレイシーンの時間
	int totalPlayFlameCount = 0;
	//高度メーター
	SingleSprite heightGauge;
	//プレイヤーの高度
	SingleSprite playerHeight;
	SingleSprite playerHeightIcon;

	//マウスカーソル
	SingleSprite mouseCursur;
	SingleSprite mouseCursurSub;

	//シーン番号
	sceneType scene = sceneType::title;

	//タイトルアニメーション
	bool isTitleAnimation = false;

	easingManager titleDragEase;
	const float dragEaseStart = -130;
	const float dragEaseEnd = (windowWidth / 2) - 70;

	easingManager titleShootEase;
	const float shootEaseStart = 1450;
	const float shootEaseEnd = (windowWidth / 2) + 70;

	const int titleEaseTime = 80;

	float titleWhiteBackAlpha = 1.0f;

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

	//最大ステージ番号
	const int maxStageNum = 3;

	//現在のステージレベル(敵を全滅させると1つ上がる)
	int nowStageLevel = 1;

	//ウェーブ表示スプライト
	SingleSprite enemyWaveBar;
	SingleSprite playerWaveIcon;
	std::list<std::unique_ptr<SingleSprite>> enemyWaveIcons;

	const float totalWaveBarLength = 500;
	const float waveBarPosX = 1180;
	float nextWaveDis = 0;

	//ステージごとの最大レベル(ここまで到達するとボス出現)
	int stageLevel = 1;

	//敵の湧き数
	int enemySpawnNum = 0;

	// stageLevel = stageNum + 2
	// stageLevel が最大まで行くとボス出現
	// ボスの HP は stageLevel + 5
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
	int countDownTime = 60;
	easingManager countDownEase;

	//チュートリアル用テキスト
	SingleSprite moveText;
	bool isMoveText = false;
	SingleSprite boostText;
	bool isBoostText = false;
	SingleSprite shotText;
	bool isShotText = false;
	SingleSprite missileText;
	bool isMissileText = false;
	SingleSprite shootingText;
	bool isShootingText = false;

	//リザルト画面のイージングフラグ
	//背景＆テキスト
	bool isMoveScreen = false;
	//背景
	bool isScreenEase = false;
	//テキスト
	bool isTextEase = false;

	easingManager resultScreenEase;
	easingManager clearTextEase;
	easingManager overTextEase;

	//リザルト画面ボタンのイージング
	easingManager ButtonEase_y;
	easingManager ButtonEase_x;
	easingManager selectEase;

	//今どっちのボタンを選んでいるのか
	//-1：select , 1：title
	int isSelectOrTitle = -1;
	bool isMoveSelectIcon = false;

	bool isPushTitle = false;

	//チュートリアル
	bool isTutorial = false;

	bool isLoadStage = false;
};