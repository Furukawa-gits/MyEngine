#pragma once
#include"primitiveEnemy.h"

class ShotEnemy :
	public primitiveEnemy
{
public:
	ShotEnemy();
	~ShotEnemy();

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
	void shot();

	//�ˌ��p
	bool isShot = false;//�ˌ��t���O
	int nextShotMaxTime = 0;//���̎ˌ��܂ł̑ҋ@����
	int nextShotTime = 0;//���݂̑ҋ@����
	bool isInRange = false;//�˒��͈͓����ǂ���

	//�e
	std::list<std::unique_ptr<NormalBullet>> normalBullets;

};