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
	void setParameter();

	//更新
	void updata();

	//描画(3D)
	void draw3D();

	//描画(2D)
	void draw2D();

private:
	//ステージ読み込み
	bool loadStage();

private:
	//ステージアイコン
	SingleSprite stage[5];
	//セレクトアイコン
	SingleSprite selects[3];
	float selectIconSizeX = 150;
	//プレイスタートボタン
	SingleSprite spaceStart;
	//チュートリアル誘導
	SingleSprite toTutorial;
	//ステージアイコンのイージング
	easingManager stageIconEase;
	//ステージアイコンを動かしているかどうか
	bool isMoveStageIcon = false;
	//スタートボタンのイージング
	easingManager playStartButtonEaseY;
	easingManager playStartButtonEaseX;
	//入力フラグ
	bool isPushStart = false;
	//ステージ読み込みフラグ
	bool isLoadStage = false;
};