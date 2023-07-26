#pragma once
#include"PrimitiveBullet.h"
#include"Enemy.h"
#include"../Math/SplineCurve.h"

#include<memory>

//追尾弾
class Missile :
	public PrimitiveBullet
{
private:

	/// <summary>
	/// 弾の本体パーティクル
	/// <para>パーティクル側の寿命フレームで消えない</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	//パーティクル生成用のカウント
	int particleCount = 0;

	//敵に当たるまでの最大フレーム
	static const int toEnemyMaxFrame = 30;

	int aliveFrame = 0;

	//スプライン曲線
	SplineCurve missileCurve;

	std::vector<RKDVector3> missilePositionts;

public:

	Enemy* enemyPointer = nullptr;

	bool isTargetSet = false;

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

	void init(XMFLOAT4 motherColor, XMFLOAT4 childColor) override;
	void updata() override;
	void draw(directX* directx) override;

	void setPenemy(Enemy* enemy);
	void start(XMFLOAT3 start_pos);
	void particleUpdata();
};