#pragma once
#include"Enemy.h"
#include"../3D/3Dobject.h"

//通常弾
class bullet
{
public:
	float count = 0;
	object3D object;
	XMFLOAT3 bullet_vec = { 0.0f,0.0f,0.0f };
	bool Isarive = false;

	void init(int index);
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void checkhit(Enemy* enemy);
	void update();
	void draw();

	Sphere bullet_collision;

private:
	Viewes bullet_view;
};


//ホーミングミサイル
class Missile
{
public:
	Enemy* P_enemy = nullptr;
	object3D object;
	XMFLOAT3 bullet_vec = { 0.0f,0.0f,0.0f };
	bool Isarive = false;
	bool IsTarget_set = false;

	void init(int index);
	void setPenemy(Enemy* enemy);
	void start(XMFLOAT3 start_pos);
	void checkhit();
	void update();
	void draw();
	
	Sphere missile_collision;

	XMFLOAT3 bullet_vec_index[8] = {
		{  10.0f,  0.0f,0.0f },
		{ -10.0f,  0.0f,0.0f },
		{   0.0f, 10.0f,0.0f },
		{   0.0f,-10.0f,0.0f },
		{  10.0f, 10.0f,0.0f },
		{  10.0f,-10.0f,0.0f },
		{ -10.0f, 10.0f,0.0f },
		{ -10.0f,-10.0f,0.0f }
	};

private:
	Viewes misslie_view;
};