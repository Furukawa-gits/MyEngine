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

	void init(dxinput* input, TexManager* tex, directX* directx);

	void Move();

	void checkplayerbullet(Enemy* enemy);

	void update();

	void reset();

	void draw_3d(directX* directx, TexManager* tex);

	void draw_2d(directX* directx, TexManager* tex);


	//3dオブジェクト
	Object3d_FBX* Player_object = nullptr;
	Model* Player_model = nullptr;

	//追従カメラ
	FollowCamera* followcamera = nullptr;

	//回転量
	XMFLOAT3 objectRot = { 0,0,0 };

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