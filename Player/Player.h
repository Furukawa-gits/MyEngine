#pragma once
#include<memory>
#include<list>
#include"../3D/Collision.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"
#include"Bullets.h"


const int MaxPlayerMissileNum = 8;

class Player
{
public:
	Player();
	~Player();

	void init(dxinput* input, directX* directx);

	void Move();

	void boostMove();

	void cameraMove();

	void playerClearMove();

	void playerDeathMove();

	void setStaging(bool isclear);

	void update();

	void targetUpdate();

	void addMissile(Enemy* enemy);

	void reset();

	void draw3D(directX* directx);

	void draw2D(directX* directx,int targetnum);

	//プレイヤーの位置を取得
	XMFLOAT3 getPlayerPos() { return playerObject->getPosition(); }

	//プレイヤーの正面ベクトルを取得
	XMFLOAT3 getPlayerFront() { return followCamera->getFrontVec(); }

private:
	static std::unique_ptr<Model> playerModel;

	//クォータニオン回転行列
	Quaternion qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);
	Quaternion qLocalCamera = quaternion(XMFLOAT3(0, 0, 1), 0);

	//クォータニオン回転角度
	float roll = 0.0f;
	float pitch = 0.0f;
	float yow = 0.0f;

	//roll回転総量
	float totalRoll = 0.0f;

	//移動スピード
	float moveSpeed = 0.3f;
	const float defaultMoveSpeed = 0.3f;
	const float boostMoveSpeed = 2.0f;

	SingleSprite targetSecond;
	SingleSprite targetThird;

	SingleSprite remainingMissileNum[9];

	float yowRotateSpeedPositive = 0.0f;//ヨー回転(正方向)
	float yowRotateSpeedNegative = 0.0f;//ヨー回転(負方向)
	float pitchRotateSpeedPositive = 0.0f;//ピッチ回転(正方向)
	float pitchRotateSpeedNegative = 0.0f;//ピッチ回転(負方向)
	float addRotateSpeed = 0.002f;//加算量
	float subRotateSpeed = -0.002f;//減算量
	float limitRotateSpeed = 0.02f;//最大回転速度

	//マウスカーソル固定値
	const int mouseOffsetX = 640;
	const int mouseOffsetY = 360;

	//基準回転軸
	const XMFLOAT3 unitX = { 1,0,0 };
	const XMFLOAT3 unitY = { 0,1,0 };
	const XMFLOAT3 unitZ = { 0,0,1 };

	const int maxArmorTime = 300;

public:
	//3dオブジェクト
	Object3d_FBX* playerObject = nullptr;

	//追従カメラ
	FollowCamera* followCamera = nullptr;

	//ターゲットスプライト
	SingleSprite targetFirst;
	int targetCount = 0;
	int rockOnCount = 0;
	bool isRockOn = false;

	//Hpスプライト
	std::vector<SingleSprite> HPUI[10];

	//HPゲージ
	SingleSprite HPGaugeBar;
	SingleSprite boostGaugeBar;
	SingleSprite gaugeFrame;

	//ダメージ表現
	SingleSprite damage;

	//撃墜エフェクト
	std::list<std::unique_ptr<SingleParticle>> bomParticles;	//爆発
	std::list<std::unique_ptr<SingleParticle>> smokeParticles;	//煙

	//弾
	std::list<std::unique_ptr<bullet>> bulletsList;
	std::list<std::unique_ptr<Missile>> missilesList;

	//入力
	static dxinput* input;

	Sphere playerCollision;

	//生存
	bool isArive = false;

	int playerHP = 10;

	//ブースト
	bool isBoost = false;
	float boostGauge = 0;
	const float maxBoostGauge = 300;

	//無敵時間フラグ
	bool isArmor = false;
	int armorTime = 0;

	//描画しないフラグ
	int isInvisible = -1;

	//停止フラグ
	bool isStop = false;

	//カメラが動いた総量
	int cameraMoveCount = 0;

	//演出セットフラグ
	bool isStagingSet = false;

	//クリア演出時間
	bool isClearStaging = false;
	int clearTime = 0;
	const int maxClearTime = 200;

	//ゲームオーバー演出
	bool isOverStaging = false;
	int fallCount = 0;
	const int maxFallCount = 360;
	XMFLOAT3 fallRot = { 0,0,0 };
	XMFLOAT3 fallScale = { 0,0,0 };

	//射撃フラグ(チュートリアル用)
	bool isNormalShot = false;
	bool isHomingMissile = false;
};