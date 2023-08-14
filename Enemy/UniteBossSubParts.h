#pragma once
#include"primitiveEnemy.h"

class uniteBossSubParts :
	public primitiveEnemy
{
public:
	uniteBossSubParts();
	~uniteBossSubParts();

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
};