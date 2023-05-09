#pragma once
#include"primitiveScene.h"

class resultScene
	:public primitiveScene
{
public:
	//�R���X�g���N�^
	resultScene();
	~resultScene();

	//���\�[�X�̓ǂݍ���
	void loadResources() override;

	//������
	void initialize() override;

	//�p�����[�^�̃��Z�b�g
	void setParameter() override;

	//�X�V
	void updata() override;

	//�w�i�`��
	void drawBack() override;

	//�`��(3D)
	void draw3D() override;

	//�`��(2D)
	void draw2D() override;

private:
	//�N���A�V�[��
	void clearScene();

	//�I�[�o�[�V�[��
	void overScene();

private:
	//���U���g���
	SingleSprite resultScreen[2];
	//�N���A���
	SingleSprite clearText;
	//�Q�[���I�[�o�[���
	SingleSprite overText;
	//�^�C�g���E�Z���N�g�{�^��
	SingleSprite titleButton;
	SingleSprite selectButton;
	//����{�^��
	SingleSprite decisionButton;
	float resultIconSizeX = 150;

	//�}�E�X���W
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	//�}�E�X�J�[�\��
	SingleSprite mouseCursur;
	SingleSprite mouseCursurSub;

	//���U���g��ʂ̃C�[�W���O�t���O
	//�w�i���e�L�X�g
	bool isMoveScreen = false;
	//�w�i
	bool isScreenEase = false;
	//�e�L�X�g
	bool isTextEase = false;

	easingManager resultScreenEase;
	easingManager clearTextEase;
	easingManager overTextEase;

	//���U���g��ʃ{�^���̃C�[�W���O
	easingManager ButtonEase_y;
	easingManager ButtonEase_x;
	easingManager selectEase;

	bool isMoveSelectIcon = false;

	bool isPushTitle = false;
};