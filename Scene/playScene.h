#pragma once
#include "primitiveScene.h"

class playScene
	:public primitiveScene
{
public:
	//コンストラクタ
	playScene();
	~playScene();

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
	//ウェーブバー描画
	void drawNowWave();

	//ミニマップ描画
	void drawPositionUI();

	//ホーミングターゲットのセッティング
	void checkHitPlayerTarget();

	//当たり判定関数
	void checkHitEverything();

	//カウントダウン
	void countDown();

	//チュートリアル
	void tutorial();

private:
	//スカイドームの回転
	float skyShpereRotY = 0.0002f;

	//瓦礫リソース
	std::unique_ptr<Model> rubble01;
	std::unique_ptr<Model> rubble02;

	const int maxRubbleNum = 20;

	std::list<std::unique_ptr<object3dFBX>> rubbles_1;
	std::list<std::unique_ptr<object3dFBX>> rubbles_2;

	//演出用カメラ
	std::unique_ptr<Camera> stagingCamera;

	//カウントダウン・スタートアイコン
	SingleSprite countDownSprite[3];
	SingleSprite playStart;

	//プレイシーンの時間
	int totalPlayFlameCount = 0;

	//高度メーター
	SingleSprite heightGauge;

	//プレイヤーの高度
	SingleSprite playerHeight;
	SingleSprite playerHeightIcon;

	int targetnum = 0;

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

	//ボス出現フラグ
	bool isBoss = false;
};