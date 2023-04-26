#pragma once
#include"primitiveScene.h"

class selectScene
	:public primitiveScene
{
public:
	//���\�[�X�̓ǂݍ���
	void loadResources();

	//������
	void initialize();

	//�p�����[�^�̃��Z�b�g
	void reSetScene();

	//�X�V
	void updata();

	//�`��(3D)
	void draw3D();

	//�`��(2D)
	void draw2D();

private:
	//�X�e�[�W�A�C�R��
	SingleSprite stage[5];
	//�Z���N�g�A�C�R��
	SingleSprite selects[3];
	float selectIconSizeX = 150;
	//�v���C�X�^�[�g�{�^��
	SingleSprite spaceStart;
	//�X�e�[�W�A�C�R���̃C�[�W���O
	easingManager stageIconEase;
	//�X�e�[�W�A�C�R���𓮂����Ă��邩�ǂ���
	bool isMoveStageIcon = false;
	//�X�^�[�g�{�^���̃C�[�W���O
	easingManager startButtonEase_y;
	easingManager startButtonEase_x;
	//���̓t���O
	bool isPushStart = false;
};