#pragma once
#include"primitiveScene.h"

class selectScene
	:public primitiveScene
{
public:
	selectScene();
	~selectScene();

	//リソースの読み込み
	void loadResources() override;

	//初期化
	void initialize() override;

	//パラメータのリセット
	void setParameter() override;

	//更新
	void updata() override;

	//背景描画
	void drawBack() override;

	//描画(3D)
	void draw3D() override;

	//描画(2D)
	void draw2D() override;

private:
	//ステージ読み込み
	bool loadStage();

private:
	//マウス座標
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	//マウスカーソル
	SingleSprite mouseCursur;
	SingleSprite mouseCursurSub;
	//背景
	SingleSprite selectBack;
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