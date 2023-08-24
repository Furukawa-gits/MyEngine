#pragma once
#include<memory>
#include<list>
#include"../3D/Collision.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"
#include"BulletManager.h"
#include"PlayerHitPointManager.h"
#include"../3D/Object3DSingleLine.h"


const int MaxPlayerMissileNum = 8;

class Player
{
public:
	Player();
	~Player();

	void init(dxinput* input, directX* directx);

	void loadUISprite();

	void move();

	void outArea();

	void boostMove();

	void quaternionMove();

	void playerClearMove();

	void playerDeathMove();

	void setStaging(bool isclear);

	void updata();

	void targetUpdata();

	void addMissile(Enemy* enemy, int& targetnum);

	void reset();

	void draw3D(directX* directx);

	void draw2D(directX* directx, int targetnum);

	/// <summary>
	/// ミニマップ描画
	/// </summary>
	/// <param name="directx">directX_Bace</param>
	void drawMiniMapIcon(directX* directx);

	//プレイヤーの位置を取得
	XMFLOAT3 getPlayerPos() { return playerObject->getPosition(); }

	//プレイヤーの正面ベクトルを取得
	XMFLOAT3 getPlayerFront() { return followCamera->getFrontVec(); }

	//ターゲットアイコンの座標を取得
	XMFLOAT3 getTargetPosition() { return targetFirst.position; }

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
	float moveSpeed = 0.0f;
	const float defaultMoveSpeed = 0.4f;
	const float boostMoveSpeed = 2.5f;

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

public:
	//3dオブジェクト
	object3dFBX* playerObject = nullptr;

	//追従カメラ
	FollowCamera* followCamera = nullptr;

	//場外判定用変数
	const XMFLOAT3 groundPosition = { 0,-180,0 };		//地面の座標(当たり判定用)
	float lengthForPlayerPosition;						//原点からの距離
	bool isOutArea = false;								//エリア外にいるかどうか
	int outAreaCount = 0;								//エリア外にいる時間(戻るとリセット)
	bool isCautionDraw = false;							//エリア外警告の描画フラグ
	SingleSprite outAreaCaution;						//エリア外警告

	//ターゲットスプライト
	SingleSprite targetFirst;
	int targetCount = 0;
	int rockOnCount = 0;
	bool isRockOn = false;
	SingleSprite targetSecond;
	SingleSprite targetThird;

	//ロックオンモードのゲージ
	SingleSprite rockonGauge[2];

	//ゲージUI
	SingleSprite boostGaugeBar;
	SingleSprite gaugeFrame;

	/// <summary>
	/// ミニマップ上のアイコン
	/// </summary>
	SingleSprite miniMapPlayer;

	//ジェットエンジンエフェクト
	int moveParticlesCount = 0;

	//弾
	std::unique_ptr<BulletManager> bulletManager;

	//入力
	static dxinput* input;

	//当たり判定
	Sphere playerCollision;

	//HPマネージャー
	PlayerHitPointManager hitPointManager;

	//ブースト
	bool isBoost = false;
	float boostGauge = 0;
	const float maxBoostGauge = 300;

	//描画しないフラグ
	int isInvisible = -1;

	//停止フラグ
	bool isStop = false;

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

	//ミサイルの発射レート
	bool isShotMissile = false;
	const int nextMissileTime = 15;
	int waitMissileTime = nextMissileTime;

	//チュートリアル用変数
	//フラグ
	bool isBoostTutorial = false;
	bool isNormalShot = false;
	bool isHomingMissile = false;
	//カウント
	int cameraMoveCount = 0;//カメラが動いた総量
	int normalShotCount = 0;//通常弾を撃った数
	int missileCount = 0;	//ミサイルを撃った数
	int boostCount = 0;		//加速した回数

	//-------------------検証用------------------------------

	
};