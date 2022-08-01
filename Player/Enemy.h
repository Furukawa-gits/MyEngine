#pragma once
//#include"Player.h"
#include"../2D/Sprite.h"
#include"../3D/3Dobject.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"

struct enemy_bullet
{
public:
	int count = 0;
	XMFLOAT3 bullet_vec = { 0.0f,0.0f,0.0f };
	bool Isarive = false;

	void init(int index);
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void update();
	void draw();

	Sphere bullet_collision;
private:
};

class Enemy
{
public:
	//--------------------使わない奴(書き換えたときに随時追加)--------------------
	object3D_obj enemy;
	Viewes enemy_view;
	Viewes enemy_view2;
	Viewes enemy_view3;
	enemy_bullet bullet;
	bool Isshot = false;
	int enemy_move_Pat = 1;
	int move_flag = 1;
	XMFLOAT3 enemy_action = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Shot_Vec = { 0.0f,0.0f,0.0f };
	XMVECTOR enemy_sc_pos;
	bool IsRockon_draw = false;
	Sphere enemy_collision;

	//--------------------使うやつ--------------------
	bool Isarive = false;//生存
	bool Istarget_set = false;//狙われているかどうか
	SingleSprite Rock_Target;//マーカー

	//体力
	int HP = 1;
	//座標・速度
	XMFLOAT3 startPosition = {};
	XMFLOAT3 position = {};
	float enemy_speed = 0.05f;
	//オブジェクト
	Model* testCube = nullptr;
	Object3d_FBX* testObject = nullptr;
	//自機に向かっていくカウント
	int homingCount = 0;

	void init(int enemy_index, int enemy_bullet_index);

	void set(XMFLOAT3 pos);

	void reSet();

	void update(XMFLOAT3 Player_pos);

	void chase(XMFLOAT3 pPos);

	void isHitTarget(XMFLOAT2 targetpos,bool istarget);

	void isHitShot(XMFLOAT2 targetpos);

	void draw3D(directX* directx);

	void drawSp(directX* directx);
};