#pragma once
#include"PrimitiveBullet.h"

class NormalBullet :
	public PrimitiveBullet
{
public:
	//�e�̍ő吶������
	const int maxBulletCount = 100;

	//���x
	float bulletSpeed = 3.0f;

	NormalBullet();
	~NormalBullet();

	void init(XMFLOAT4 motherColor, XMFLOAT4 childColor) override;
	void updata() override;
	void draw(directX* directx) override;

	void set(XMFLOAT3 start_pos, XMFLOAT3 Target);
};