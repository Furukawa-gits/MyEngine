#pragma once
#include"primitiveEnemy.h"

class TutorialEnemy :
	public primitiveEnemy
{
public:
	TutorialEnemy();
	~TutorialEnemy();

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

	//�`���[�g���A���p�Ȃ̂Ń{�X�p���ď����̓i�V
	//void deathMoveBoss() override;

	//UI�X�v���C�g�X�V
	void updataSprite() override;

	//3D�`��
	void draw3D() override;

	//2D�`��
	void draw2D() override;

	//2D�`��(�~�j�}�b�v)
	void drawMiniMapIcon() override;
};