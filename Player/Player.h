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


	//3d�I�u�W�F�N�g
	Object3d_FBX* Player_object = nullptr;
	Model* Player_model = nullptr;

	//�Ǐ]�J����
	FollowCamera* followcamera = nullptr;

	//��]��
	XMFLOAT3 objectRot = { 0,0,0 };

	float up = 0.0f;
	float right = 0.0f;

	float pitch = 0.0f;
	float yow = 0.0f;
	float roll = 0.0f;

	//�^�[�Q�b�g�X�v���C�g
	SingleSprite target;
	int Target_count = 0;
	int Rockon_count = 0;
	bool Isrockon = false;

	//Hp�X�v���C�g
	SingleSprite hp[10];

	//�e
	bullet player_bullet[MaxPlayerBulletNum];
	Missile player_missiale[MaxPlayerMissileNum];

	//����
	static dxinput* input;

	Sphere player_collision;


	//����
	bool Isarive = false;

	int HP = 10;

};