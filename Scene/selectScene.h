#pragma once
#include"primitiveScene.h"

class selectScene
	:public primitiveScene
{
public:
	//リソースの読み込み
	void loadResources();

	//初期化
	void initialize();

	//パラメータのリセット
	void reSetScene();

	//更新
	void updata();

	//描画(3D)
	void draw3D();

	//描画(2D)
	void draw2D();

private:
	//ステージアイコン
	SingleSprite stage[5];
	//セレクトアイコン
	SingleSprite selects[3];
	float selectIconSizeX = 150;
	//プレイスタートボタン
	SingleSprite spaceStart;
	//ステージアイコンのイージング
	easingManager stageIconEase;
	//ステージアイコンを動かしているかどうか
	bool isMoveStageIcon = false;
	//スタートボタンのイージング
	easingManager startButtonEase_y;
	easingManager startButtonEase_x;
	//入力フラグ
	bool isPushStart = false;
};