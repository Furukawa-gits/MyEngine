#pragma once
#include"primitiveScene.h"

class selectScene
	:public primitiveScene
{
public:
	selectScene();

	//���\�[�X�̓ǂݍ���
	void loadResources() override;

	//������
	void initialize() override;

	//�p�����[�^�̃��Z�b�g
	void setParameter() override;

	//�X�V
	void updata() override;

	//�`��(3D)
	void draw3D() override;

	//�`��(2D)
	void draw2D() override;

private:
	//�X�e�[�W�ǂݍ���
	bool loadStage();

private:
	//�}�E�X���W
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	//�}�E�X�J�[�\��
	SingleSprite mouseCursur;
	SingleSprite mouseCursurSub;
	//�w�i
	SingleSprite selectBack;
	//�X�e�[�W�A�C�R��
	SingleSprite stage[5];
	//�Z���N�g�A�C�R��
	SingleSprite selects[3];
	float selectIconSizeX = 150;
	//�v���C�X�^�[�g�{�^��
	SingleSprite spaceStart;
	//�`���[�g���A���U��
	SingleSprite toTutorial;
	//�X�e�[�W�A�C�R���̃C�[�W���O
	easingManager stageIconEase;
	//�X�e�[�W�A�C�R���𓮂����Ă��邩�ǂ���
	bool isMoveStageIcon = false;
	//�X�^�[�g�{�^���̃C�[�W���O
	easingManager playStartButtonEaseY;
	easingManager playStartButtonEaseX;
	//���̓t���O
	bool isPushStart = false;
	//�X�e�[�W�ǂݍ��݃t���O
	bool isLoadStage = false;
};