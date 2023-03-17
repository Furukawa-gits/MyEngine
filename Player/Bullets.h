#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"Enemy.h"

#include<memory>

//通常弾
class bullet
{
private:
	static std::unique_ptr<Model> bulletModelS;

	/// <summary>
	/// 弾の本体となるパーティクル
	/// <para>弾の生存に依存するのでこいつは時間経過で消えない</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	/// <summary>
	/// 残像パーティクル
	/// <para>こいつらは弾の生存時間に関係ないので時間経過で消える</para>
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


//ホーミングミサイル
class Missile
{
private:
	static std::unique_ptr<Model> MissileModelS;

	/// <summary>
	/// 弾の本体となるパーティクル
	/// <para>弾の生存に依存するのでこいつは時間経過で消えない</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	/// <summary>
	/// 残像パーティクル
	/// <para>こいつらは弾の生存時間に関係ないので時間経過で消える</para>
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