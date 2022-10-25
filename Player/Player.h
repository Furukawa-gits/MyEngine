#pragma once
#include"../3D/Collision.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"Bullets.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"


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

	void update();

	void reset();

	void draw3D(directX* directx);

	void draw2D(directX* directx);


	//3dオブジェクト
	Object3d_FBX* Player_object = nullptr;
	Model* Player_model = nullptr;

	//追従カメラ
	FollowCamera* followcamera = nullptr;

	//回転量
	float yowRotateSpeedPositive = 0.0f;
	float yowRotateSpeedNegative = 0.0f;
	float pitchRotateSpeedPositive = 0.0f;
	float pitchRotateSpeedNegative = 0.0f;
	float addRotateSpeed = 0.002f;
	float subRotateSpeed = -0.002f;
	float limitRotateSpeed = 0.02f;

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
	int Target_count = 0;
	int Rockon_count = 0;
	bool Isrockon = false;

	//Hpスプライト
	SingleSprite hp[10];

	//弾
	bullet player_bullet[MaxPlayerBulletNum];
	Missile player_missiale[MaxPlayerMissileNum];

	//入力
	static dxinput* input;

	Sphere player_collision;

	//生存
	bool isArive = false;

	int HP = 10;

};