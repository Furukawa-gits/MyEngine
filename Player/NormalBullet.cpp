#include "NormalBullet.h"

NormalBullet::NormalBullet()
{
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::init(XMFLOAT4 motherColor, XMFLOAT4 childColor)
{
	//親パーティクル生成
	motherParticle = std::make_unique<SingleParticle>();
	motherParticle->generate();
	motherParticle->set(0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f, false, true);
	motherParticle->color = motherColor;

	this->childColor = childColor;

	bulletCollision.radius = 4.0f;
}

void NormalBullet::set(XMFLOAT3 start_pos, XMFLOAT3 Target)
{
	position = start_pos;

	bulletCollision.center = XMLoadFloat3(&position);

	XMFLOAT3 dis = {
		Target.x - start_pos.x,
		Target.y - start_pos.y,
		Target.z - start_pos.z,
	};

	float length = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	bulletVec = { (dis.x / length) * bulletSpeed,(dis.y / length) * bulletSpeed ,(dis.z / length) * bulletSpeed };

	isAlive = true;
}

void NormalBullet::updata()
{
	if (!isAlive)
	{
		return;
	}

	addBulletVec();

	bulletCount++;

	//一定フレームごとにパーティクルを生成
	if (bulletCount % 5 == 0)
	{
		SingleParticle newParticle;
		newParticle.generate();
		newParticle.set(30, position, { 0,0,0 }, { 0,0,0 }, 2.0f, 0.0f);
		newParticle.color = childColor;
		newParticle.isAddBlend = true;
		particleManagerOnTime::addParticle(newParticle, "effect1.png");
	}

	//弾の寿命が来たら消滅
	if (bulletCount >= maxBulletCount)
	{
		bulletCount = 0;
		motherParticle->setIsActive(false);
		isAlive = false;
	}

	//本体パーティクル更新
	motherParticle->setPosition(position);
	motherParticle->updata();

	bulletCollision.center = XMLoadFloat3(&position);
}

void NormalBullet::draw(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	motherParticle->setPiplineAddBlend();
	motherParticle->drawSpecifyTex("effect1.png");
}
