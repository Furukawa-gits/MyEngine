#pragma once
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
	//--------------------�g�����--------------------
	bool Isarive = false;//����
	bool Istarget_set = false;//�_���Ă��邩�ǂ���
	bool IsSetMissile = false;
	SingleSprite Rock_Target;//�}�[�J�[
	Sphere enemy_collision;

	//�̗�
	int HP = 1;
	//���W�E���x
	XMFLOAT3 startPosition = {};
	XMFLOAT3 position = {};
	float enemy_speed = 0.05f;
	//�I�u�W�F�N�g
	Model* testCube = nullptr;
	Object3d_FBX* testObject = nullptr;
	//���@�Ɍ������Ă����J�E���g
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