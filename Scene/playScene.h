#pragma once
#include "primitiveScene.h"

class playScene
	:public primitiveScene
{
public:
	//コンストラクタ
	playScene();

	//リソースの読み込み
	void loadResources() override;

	//初期化
	void initialize() override;

	//パラメータのリセット
	void setParameter() override;

	//更新
	void updata() override;

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
	//天球と地面
	std::unique_ptr<Model> SkyModel;
	std::unique_ptr<Model> groundModel;
	std::unique_ptr<object3dFBX> skySphere;
	std::unique_ptr<object3dFBX> groundPlane;

	//スカイドームの回転
	float skyShpereRotY = 0.0002f;

	//演出用カメラ
	Camera* stagingCamera = nullptr;

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