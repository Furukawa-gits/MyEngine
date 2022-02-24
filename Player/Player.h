#pragma once
#include"../3D/3Dobject.h"
#include"../3D/Collision.h"
#include"../2D/SpriteSingleunit.h"
#include"../Input/dxInput.h"
#include"Bullets.h"

const float Move_limit = 30.0f;

const int MaxPlayerBulletNum = 20;
const int MaxPlayerMissileNum = 4;

class Player
{
public:

	void init(dxinput* input, TexManager* tex, directX* directx);

	void Move();

	void checkplayerbullet(Enemy* enemy);

	void checkenemybullet(enemy_bullet* bullet);

	void checkrockon(Enemy& enemy);

	void update(SpriteCommon* commonsp);

	void draw(directX* directx, TexManager* tex, SpriteCommon* commonsp);


	//3d�I�u�W�F�N�g
	object3D Player_object;
	Viewes player_view;
	Viewes old_player_view;

	//�^�[�Q�b�g
	SingleSprite Target;
	int Target_count = 0;
	int Rockon_count = 0;
	bool Isrockon = false;

	//Hp�X�v���C�g
	SingleSprite hp[10];

	//�e
	bullet player_bullet[MaxPlayerBulletNum];
	Missile player_missiale[MaxPlayerMissileNum];

	//���̓|�C���^
	static dxinput* input;

	Sphere player_collision;


	//����
	bool Isarive;

	int HP = 10;

};