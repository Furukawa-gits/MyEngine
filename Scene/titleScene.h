#pragma once
#include"primitiveScene.h"

class titleScene
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
	//背景
	SingleSprite titleBack;
	//タイトル
	SingleSprite titleIconDrag;
	SingleSprite titleIconShoot;
	SingleSprite titlePlayer;
	SingleSprite titleWhiteBack;
	//スタートボタン
	SingleSprite startButton;
	//マウスカーソル
	SingleSprite mouseCursur;
	SingleSprite mouseCursurSub;
	//タイトルアニメーション
	bool isTitleAnimation = false;
	//タイトルアイコンのイージング
	easingManager titleDragEase;
	const float dragEaseStart = -130;
	const float dragEaseEnd = (windowWidth / 2) - 70;
	//タイトルアイコンのイージング
	easingManager titleShootEase;
	const float shootEaseStart = 1450;
	const float shootEaseEnd = (windowWidth / 2) + 70;
	//イージングの時間
	const int titleEaseTime = 80;
	//点滅用のアルファ値
	float titleWhiteBackAlpha = 1.0f;
	//タイトル演出フラグ
	bool isTitleStage = false;
	//スタートボタンのイージング
	easingManager titleStartButtonEaseY;
	easingManager titleStartButtonEaseX;
	//入力フラグ
	bool isPushStart = false;
};