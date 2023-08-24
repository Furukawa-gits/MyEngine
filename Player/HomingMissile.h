#pragma once
#include"PrimitiveBullet.h"
#include"Enemy.h"
#include"../Math/SplineCurve.h"
#include"../3D/Object3DSingleLine.h"

#include<memory>
#include <random>

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

	SplineCurve missileCurve;

	std::vector<RKDVector3> missilePositionts;

public:

	Enemy* enemyPointer = nullptr;

	bool isTargetSet = false;

	std::list<std::unique_ptr<Object3DSingleLine>> missileLineList;

	Missile();
	~Missile();

	void init(XMFLOAT4 motherColor, XMFLOAT4 childColor) override;
	void updata() override;
	void draw(directX* directx) override;

	void setPenemy(Enemy* enemy);
	void start(XMFLOAT3 start_pos);
	void particleUpdata();
};