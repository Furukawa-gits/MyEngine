#include "NormalBullet.h"

NormalBullet::NormalBullet()
{
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::staticInit()
{
	SingleParticle::loadTexInMap("effect1.png");
}

void NormalBullet::staticDestroy()
{
}

void NormalBullet::init(XMFLOAT4 color)
{
	//�e�p�[�e�B�N������
	motherParticle = std::make_unique<SingleParticle>();
	motherParticle->generate();
	motherParticle->set(0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3.0f, 0.0f, false, true);
	motherParticle->color = color;

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

	count++;

	//���t���[�����ƂɃp�[�e�B�N���𐶐�
	if (count % 5 == 0)
	{
		SingleParticle newParticle;
		newParticle.generate();
		newParticle.set(30, position, { 0,0,0 }, { 0,0,0 }, 2.0f, 0.0f);
		newParticle.color = { 0,1,0,1 };
		newParticle.isAddBlend = true;
		particleManagerOnTime::addParticle(newParticle, "effect1.png");
	}

	//�e�̎��������������
	if (count >= maxBulletCount)
	{
		count = 0;
		motherParticle->setIsActive(false);
		isAlive = false;
	}

	//�{�̃p�[�e�B�N���X�V
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
