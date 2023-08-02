#pragma once
#include"primitiveEnemy.h"

class RampageEnemy :
	public primitiveEnemy
{
public:
	RampageEnemy();
	~RampageEnemy();

	//������
	void init(bool isboss) override;

	//�Z�b�g
	void set(XMFLOAT3 pos) override;

	//�X�V
	void updata() override;

	//�ڏ����o
	void arrival() override;

	//����������
	void ariveMove() override;

	//���ď���
	void deathMove() override;

	//�{�X��p���ĉ��o(if�ŕЂÂ����Ȃ����x���ňႤ�̂Ō��Ă�����)
	void deathMoveBoss() override;

	//UI�X�v���C�g�X�V
	void updataSprite() override;

	//3D�`��
	void draw3D() override;

	//2D�`��
	void draw2D() override;

	//2D�`��(�~�j�}�b�v)
	void drawMiniMapIcon() override;

private:
	void rampage();

	//���˗p
	bool isShot = false;//�ˌ��t���O
	int nextShotTime = 0;//���̎ˌ��܂ł̑ҋ@����
	bool isInRange = false;//�˒��͈͓����ǂ���

	//�e
	std::list<std::unique_ptr<NormalBullet>> normalBullets;

	bool isRampageWait = true;
	int rampageWaitCount = 0;
	int bulletCount = 0;
	int nextBulletTime = 0;

	int maxBulletCount = 5;

};