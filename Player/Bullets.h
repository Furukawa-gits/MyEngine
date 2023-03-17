#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"Enemy.h"

#include<memory>

//�ʏ�e
class bullet
{
private:
	static std::unique_ptr<Model> bulletModelS;

	/// <summary>
	/// �e�̖{�̂ƂȂ�p�[�e�B�N��
	/// <para>�e�̐����Ɉˑ�����̂ł����͎��Ԍo�߂ŏ����Ȃ�</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	/// <summary>
	/// �c���p�[�e�B�N��
	/// <para>������͒e�̐������ԂɊ֌W�Ȃ��̂Ŏ��Ԍo�߂ŏ�����</para>
	/// </summary>
	std::list<std::unique_ptr<SingleParticle>> childParticles;

public:
	float count = 0;
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 bullet_vec = { 0.0f,0.0f,0.0f };

	Object3d_FBX* bulletObject = nullptr;
	float bulletSpeed = 3.0f;

	bool isArive = false;

	Sphere bulletCollision;

	bullet();
	~bullet();

	static void staticInit();
	static void staticDestroy();
	void init();
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void update();
	void addBulletVec()
	{
		position.x += bullet_vec.x;
		position.y += bullet_vec.y;
		position.z += bullet_vec.z;
	}
	void draw(directX* directx);
};


//�z�[�~���O�~�T�C��
class Missile
{
private:
	static std::unique_ptr<Model> MissileModelS;

	/// <summary>
	/// �e�̖{�̂ƂȂ�p�[�e�B�N��
	/// <para>�e�̐����Ɉˑ�����̂ł����͎��Ԍo�߂ŏ����Ȃ�</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	/// <summary>
	/// �c���p�[�e�B�N��
	/// <para>������͒e�̐������ԂɊ֌W�Ȃ��̂Ŏ��Ԍo�߂ŏ�����</para>
	/// </summary>
	std::list<std::unique_ptr<SingleParticle>> childParticles;

public:

	Enemy* enemyPointer = nullptr;
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 bulletVec = { 0.0f,0.0f,0.0f };

	Object3d_FBX* bulletObject = nullptr;
	bool isArive = false;
	bool isTargetSet = false;

	Sphere missileCollision;

	XMFLOAT3 bulletVecIndex[8] = {
		{  30.0f,  0.0f,0.0f },
		{ -30.0f,  0.0f,0.0f },
		{   0.0f, 30.0f,0.0f },
		{   0.0f,-30.0f,0.0f },
		{  30.0f, 30.0f,0.0f },
		{  30.0f,-30.0f,0.0f },
		{ -30.0f, 30.0f,0.0f },
		{ -30.0f,-30.0f,0.0f }
	};

	Missile();
	~Missile();

	static void staticInit();
	static void staticDestroy();
	void init();
	void setPenemy(Enemy* enemy);
	void start(XMFLOAT3 start_pos);
	void update();
	void addBulletVec()
	{
		position.x += bulletVec.x;
		position.y += bulletVec.y;
		position.z += bulletVec.z;
	}
	void draw(directX* directx);
};