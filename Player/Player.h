#pragma once
#include"../3D/Collision.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"
#include"Bullets.h"


const int MaxPlayerBulletNum = 20;
const int MaxPlayerMissileNum = 8;

class Player
{
public:
	Player();
	~Player();

	void init(dxinput* input, directX* directx);

	void Move();

	void cameraMove();

	void checkPlayerBullet(Enemy* enemy);

	void checkPlayerEnemy(Enemy* enemy);

	void checkPlayerEnemyBullet(Enemy* enemy);

	void update();

	void targetUpdate();

	void reset();

	void draw3D(directX* directx);

	void draw2D(directX* directx);


	//3dオブジェクト
	Object3d_FBX* playerObject = nullptr;
	Model* playerModel = nullptr;

	//追従カメラ
	FollowCamera* followCamera = nullptr;

	//回転量
	float yowRotateSpeedPositive = 0.0f;//ヨー回転(正方向)
	float yowRotateSpeedNegative = 0.0f;//ヨー回転(負方向)
	float pitchRotateSpeedPositive = 0.0f;//ピッチ回転(正方向)
	float pitchRotateSpeedNegative = 0.0f;//ピッチ回転(負方向)
	float addRotateSpeed = 0.002f;//加算量
	float subRotateSpeed = -0.002f;//減算量
	float limitRotateSpeed = 0.02f;//最大回転速度

	//基準回転軸
	const XMFLOAT3 unitX = { 1,0,0 };
	const XMFLOAT3 unitY = { 0,1,0 };
	const XMFLOAT3 unitZ = { 0,0,1 };

	//クォータニオン回転行列
	Quaternion qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);

	//クォータニオン回転角度
	float roll = 0.0f;
	float pitch = 0.0f;
	float yow = 0.0f;

	//座標
	XMFLOAT3 position = { 0,5,30 };

	//移動スピード
	float moveSpeed = 0.3f;

	//ターゲットスプライト
	SingleSprite targetFirst;
	SingleSprite targetSecond;
	SingleSprite targetThird;
	int targetCount = 0;
	int rockOnCount = 0;
	bool isRockOn = false;

	//Hpスプライト
	SingleSprite hitPointUI[10];

	//弾
	bullet playerBullet[MaxPlayerBulletNum];
	Missile playerMissiale[MaxPlayerMissileNum];

	//入力
	static dxinput* input;

	Sphere playerCollision;

	//生存
	bool isArive = false;

	int playerHP = 10;

};