#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"Enemy.h"
#include"../Math/SplineCurve.h"

#include<memory>

//ホーミングミサイル
class Missile
{
private:

	/// <summary>
	/// 弾の本体となるパーティクル
	/// <para>弾の生存に依存するのでこいつは時間経過で消えない</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	//パーティクル生成の為のカウント
	int particleCount = 0;

	//敵に到達するまでのフレーム数
	static const int toEnemyMaxFrame = 30;

	int aliveFrame = 0;

	//スプライン曲線
	SplineCurve missileCurve;

	std::vector<RKDVector3> missilePositionts;

public:

	Enemy* enemyPointer = nullptr;
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 bulletVec = { 0.0f,0.0f,0.0f };

	bool isAlive = false;
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
	/// <summary>
	/// 通常弾と違い、処理が多いのでパーティクルの更新は分ける
	/// </summary>
	void particleUpdata();
	void addBulletVec()
	{
		position.x += bulletVec.x;
		position.y += bulletVec.y;
		position.z += bulletVec.z;
	}
	void draw(directX* directx);
};