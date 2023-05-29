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
#include"../staging/Easing.h"
#include"../Player/Player.h"
#include"../Player/Enemy.h"
#include"../Player/Boss.h"
#include"../Player/uniteBoss.h"
#include"../Player/CheckHItManager.h"
#include<random>
#include<time.h>
#include<memory>
#include<list>

enum class gameSceneType
{
	title = 0,
	select = 1,
	play = 2,
	result = 3
};

class primitiveScene
{
public:
	primitiveScene();
	~primitiveScene();

	static void setStaticData(directX* Directx, dxinput* Input, Audio* Audio);

	static void finalize();

	//リソースの読み込み
	virtual void loadResources() = 0;

	//初期化
	virtual void initialize() = 0;

	//パラメータのリセット
	virtual void setParameter() = 0;

	//更新
	virtual void updata() = 0;

	//背景描画
	virtual void drawBack() = 0;

	//描画(3D)
	virtual void draw3D() = 0;

	//描画(2D)
	virtual void draw2D() = 0;

	//次のシーンに移るかどうか
	bool getIsNextScene() { return isNextScene; }

public:
	//main.cppから参照するので生ポインタ
	static directX* directx;
	static dxinput* input;
	static Audio* audio;

	//シーンの種類
	gameSceneType thisType = gameSceneType::title;

	//次のシーンに移行するフラグ
	bool isNextScene = false;

	//-------複数のシーンをまたいで使う物はここに書く-------
	//背景
	static std::unique_ptr<SingleSprite> sample_back;
	//天球と地面
	static std::unique_ptr<Model> SkyModel;
	static std::unique_ptr<Model> SkyModel2;
	static std::unique_ptr<Model> SkyModel3;
	static std::unique_ptr<Model> groundModel;
	static std::unique_ptr<object3dFBX> skySphere;
	static std::unique_ptr<object3dFBX> skySphere2;
	static std::unique_ptr<object3dFBX> skySphere3;
	static std::unique_ptr<object3dFBX> groundPlane;
	//ライト
	static Light* light;
	//敵リスト
	static std::list<std::unique_ptr<Enemy>> enemyList;
	//通常ボス
	static std::unique_ptr<Boss> normalBoss;
	//ユニットボス
	static std::unique_ptr<uniteBoss> UniteBoss;
	//プレイヤー
	static std::unique_ptr<Player> playerPointer;
	//ウェーブ表示スプライト
	static std::unique_ptr<SingleSprite> enemyWaveBar;
	static std::unique_ptr<SingleSprite> playerWaveIcon;
	static std::list<std::unique_ptr<SingleSprite>> enemyWaveIcons;
	const float totalWaveBarLength = 500;
	const float waveBarPosX = 1180;
	//ステージ番号
	static int stageNum;
	//最大ステージ番号
	static const int maxStageNum = 3;
	//ステージアイコンの基準座標
	static XMFLOAT3 stageIconOffset;
	//現在のステージレベル(敵を全滅させると1つ上がる)
	static int nowStageLevel;
	//ステージごとの最大レベル(ここまで到達するとボス出現)
	static int maxStageLevel;
	//チュートリアル
	static bool isTutorial;
	//クリアかオーバーか(true:クリア/false:オーバー)
	static bool isClearOrOver;
	//タイトルかセレクトか(true:セレクト/false:タイトル)
	static bool isSelectOrTitle;
	//アイコン同士の距離
	static float nextWaveDis;
};