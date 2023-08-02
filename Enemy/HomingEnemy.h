#pragma once
#include"primitiveEnemy.h"

class HomingEnemy :
	public primitiveEnemy
{
public:
	HomingEnemy();
	~HomingEnemy();

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
	void homing();

	//�ǔ����ˌ��p
	bool isChase = false;//�ǐՃt���O
	bool isWait = false;//�ҋ@�t���O
	int chaseCount = 0;//�ǔ��J�E���g
	int waitCount = 0;//�ҋ@�J�E���g

	//�ˌ��p
	bool isShot = false;//�ˌ��t���O
	int nextShotTime = 0;//���̎ˌ��܂ł̑ҋ@����
	bool isInRange = false;//�˒��͈͓����ǂ���

	//�e
	std::list<std::unique_ptr<NormalBullet>> normalBullets;
};