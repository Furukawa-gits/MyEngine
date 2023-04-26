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
#include<random>
#include<time.h>
#include<memory>
#include<list>

enum class sceneType
{
	title = 0,
	select = 1,
	play = 2,
	result = 3
};

class primitiveScene
{
public:
	static void setStaticData(directX* Directx, dxinput* Input, Audio* Audio);

	//リソースの読み込み
	virtual void loadResources();

	//初期化
	virtual void initialize();

	//パラメータのリセット
	virtual void setParameter();

	//更新
	virtual void updata();

	//描画(3D)
	virtual void draw3D();

	//描画(2D)
	virtual void draw2D();

	//次のシーンに移るかどうか
	bool getIsNextScene() { return isNextScene; }

public:
	//main.cppから参照するので生ポインタ
	static directX* directx;
	static dxinput* input;
	static Audio* audio;

	//次のシーンに移行するフラグ
	bool isNextScene = false;

	//複数のシーンをまたいで使う物はここに書く
	//ステージ番号
	static int stageNum;
	//最大ステージ番号
	static const int maxStageNum = 3;
	//現在のステージレベル(敵を全滅させると1つ上がる)
	static int nowStageLevel;
	//ステージごとの最大レベル(ここまで到達するとボス出現)
	static int maxStageLevel;
	//チュートリアル
	static bool isTutorial;
};