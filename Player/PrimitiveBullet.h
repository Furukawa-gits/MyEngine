#pragma once
#include"../2D/Sprite.h"
#include"../3D/Collision.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"../Particle/particleManagerOnTime.h"

class PrimitiveBullet
{
public:
	static void staticInit();

	PrimitiveBullet();
	~PrimitiveBullet();

	virtual void init(XMFLOAT4 motherColor, XMFLOAT4 childColor) = 0;

	virtual void updata() = 0;

	virtual void draw(directX* dx) = 0;

	void addBulletVec()
	{
		position.x += bulletVec.x;
		position.y += bulletVec.y;
		position.z += bulletVec.z;
	}

public:
	//�J�E���g(�������ԁE�p�[�e�B�N�������p)
	int bulletCount = 0;

	//���W�E�i�s�x�N�g��
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 bulletVec = { 0.0f,0.0f,0.0f };

	//�����t���O
	bool isAlive = false;

	//�R���W����
	Sphere bulletCollision;

	//�F
	XMFLOAT4 childColor = { 0,0,0,0 };

protected:
	//�e�{�̂̃p�[�e�B�N��
	std::unique_ptr<SingleParticle> motherParticle;
};