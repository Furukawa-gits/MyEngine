#pragma once
#include"primitiveScene.h"

class titleScene
	:public primitiveScene
{
public:
	//�R���X�g���N�^
	titleScene();

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
	//�}�E�X���W
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	//�w�i
	SingleSprite titleBack;
	//�^�C�g��
	SingleSprite titleIconDrag;
	SingleSprite titleIconShoot;
	SingleSprite titlePlayer;
	SingleSprite titleWhiteBack;
	//�X�^�[�g�{�^��
	SingleSprite startButton;
	//�}�E�X�J�[�\��
	SingleSprite mouseCursur;
	SingleSprite mouseCursurSub;
	//�^�C�g���A�j���[�V����
	bool isTitleAnimation = false;
	//�^�C�g���A�C�R���̃C�[�W���O
	easingManager titleDragEase;
	const float dragEaseStart = -130;
	const float dragEaseEnd = (windowWidth / 2) - 70;
	//�^�C�g���A�C�R���̃C�[�W���O
	easingManager titleShootEase;
	const float shootEaseStart = 1450;
	const float shootEaseEnd = (windowWidth / 2) + 70;
	//�C�[�W���O�̎���
	const int titleEaseTime = 80;
	//�_�ŗp�̃A���t�@�l
	float titleWhiteBackAlpha = 1.0f;
	//�^�C�g�����o�t���O
	bool isTitleStage = false;
	//�X�^�[�g�{�^���̃C�[�W���O
	easingManager titleStartButtonEaseY;
	easingManager titleStartButtonEaseX;
	//���̓t���O
	bool isPushStart = false;
};