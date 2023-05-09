#pragma once
#include"primitiveScene.h"

class resultScene
	:public primitiveScene
{
public:
	//コンストラクタ
	resultScene();
	~resultScene();

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
	//クリアシーン
	void clearScene();

	//オーバーシーン
	void overScene();

private:
	//リザルト画面
	SingleSprite resultScreen[2];
	//クリア画面
	SingleSprite clearText;
	//ゲームオーバー画面
	SingleSprite overText;
	//タイトル・セレクトボタン
	SingleSprite titleButton;
	SingleSprite selectButton;
	//決定ボタン
	SingleSprite decisionButton;
	float resultIconSizeX = 150;

	//マウス座標
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	//マウスカーソル
	SingleSprite mouseCursur;
	SingleSprite mouseCursurSub;

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

	bool isMoveSelectIcon = false;

	bool isPushTitle = false;
};