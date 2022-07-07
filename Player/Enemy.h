#pragma once
//#include"Player.h"
#include"../3D/Collision.h"
#include"../3D/3Dobject.h"
#include"../2D/SpriteCommon.h"
#include"../2D/SpriteSingleunit.h"

struct enemy_bullet
{
public:
	int count = 0;
	object3D_obj object;
	XMFLOAT3 bullet_vec = { 0.0f,0.0f,0.0f };
	bool Isarive = false;

	void init(int index);
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void update();
	void draw();

	Sphere bullet_collision;

private:
	Viewes bullet_view;
};

class Enemy
{
public:
	object3D_obj enemy;
	Viewes enemy_view;
	Viewes enemy_view2;
	Viewes enemy_view3;
	enemy_bullet bullet;
	SingleSprite Rock_Target;
	bool Istarget_set = false;
	bool IsRockon_draw = false;
	bool Isarive = false;
	bool Isshot = false;
	int enemy_move_Pat = 1;
	int move_flag = 1;
	float enemy_speed = 1.5;
	XMFLOAT3 enemy_action = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Shot_Vec = { 0.0f,0.0f,0.0f };
	XMVECTOR enemy_sc_pos;

	Sphere enemy_collision;

	void init(int enemy_index,int enemy_bullet_index, TexManager* tex);
	void setenemy(int move_Pat, XMFLOAT3 position);
	void update(SpriteCommon* commonsp, XMFLOAT3 Player_pos);
	void draw(directX* directx, TexManager* tex, SpriteCommon* commonsp);
};