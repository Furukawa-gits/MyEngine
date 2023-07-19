#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../Base/DirectX_Base.h"
#include"Enemy.h"
#include"../Math/SplineCurve.h"

#include<memory>

//�z�[�~���O�~�T�C��
class Missile
{
private:

	/// <summary>
	/// �e�̖{�̂ƂȂ�p�[�e�B�N��
	/// <para>�e�̐����Ɉˑ�����̂ł����͎��Ԍo�߂ŏ����Ȃ�</para>
	/// </summary>
	std::unique_ptr<SingleParticle> motherParticle;

	//�p�[�e�B�N�������ׂ̈̃J�E���g
	int particleCount = 0;

	//�G�ɓ��B����܂ł̃t���[����
	static const int toEnemyMaxFrame = 30;

	int aliveFrame = 0;

	//�X�v���C���Ȑ�
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
	/// �ʏ�e�ƈႢ�A�����������̂Ńp�[�e�B�N���̍X�V�͕�����
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