#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"../Particle/particleManagerOnTime.h"

class NormalBullet
{
private:
	/// <summary>
	/// 弾の本体となるパーティクル
	/// <para>弾の生存に依存するのでこいつは時間経過で消えない</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

public:
	//弾の生存時間
	int count = 0;
	const int maxBulletCount = 100;

	//座標・進行ベクトル・速度
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 bulletVec = { 0.0f,0.0f,0.0f };
	float bulletSpeed = 3.0f;

	//生存フラグ
	bool isAlive = false;

	//コリジョン
	Sphere bulletCollision;

	//色
	XMFLOAT4 traceParticleColor = { 0,0,0,0 };

	NormalBullet();
	~NormalBullet();

	static void staticInit();
	static void staticDestroy();
	void init(XMFLOAT4 color);
	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
	void updata();
	void addBulletVec()
	{
		position.x += bulletVec.x;
		position.y += bulletVec.y;
		position.z += bulletVec.z;
	}
	void draw(directX* directx);
};