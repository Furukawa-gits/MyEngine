#pragma once
#include"../3D/Collision.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"Bullets.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"

const float Move_limit = 30.0f;

const int MaxPlayerBulletNum = 20;
const int MaxPlayerMissileNum = 5;

class Player
{
public:

	void init(dxinput* input, directX* directx);

	void Move();

	void cameraMove();

	void checkPlayerBullet(Enemy* enemy);

	void checkPlayerEnemy(Enemy* enemy);

	void update();

	void reset();

	void draw_3d(directX* directx);

	void draw_2d(directX* directx);


	//3dオブジェクト
	Object3d_FBX* Player_object = nullptr;
	Model* Player_model = nullptr;

	//追従カメラ
	FollowCamera* followcamera = nullptr;

	//回転量
	float yowRotateSpeed = 0.0f;
	float pitchRotateSpeed = 0.0f;
	float addRotateSpeed = 0.01f;
	float subRotateSpeed = -0.002f;
	float limitRotateSpeed = 0.05f;

	//基準回転軸
	const XMFLOAT3 unitX = { 1,0,0 };
	const XMFLOAT3 unitY = { 0,1,0 };
	const XMFLOAT3 unitZ = { 0,0,1 };

	Quaternion qLocal = quaternion(XMFLOAT3(0, 0, 1), 0);

	//座標
	XMFLOAT3 position = { 0,5,30 };

	float up = 0.0f;
	float right = 0.0f;

	float pitch = 0.0f;
	float yow = 0.0f;
	float roll = 0.0f;

	//ターゲットスプライト
	SingleSprite target;
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
	bool Isarive = false;

	int HP = 10;

};