#pragma once
#include"primitiveScene.h"

class titleScene
	:public primitiveScene
{
public:
	//コンストラクタ
	titleScene();
	~titleScene();

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
	//マウス座標
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
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