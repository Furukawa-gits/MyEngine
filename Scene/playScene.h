#pragma once
#include "primitiveScene.h"

class playScene
	:public primitiveScene
{
public:
	//�R���X�g���N�^
	playScene();
	~playScene();

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
	//�E�F�[�u�o�[�`��
	void drawNowWave();

	//�~�j�}�b�v�`��
	void drawPositionUI();

	//�z�[�~���O�^�[�Q�b�g�̃Z�b�e�B���O
	void checkHitPlayerTarget();

	//�����蔻��֐�
	void checkHitEverything();

	//�J�E���g�_�E��
	void countDown();

	//�`���[�g���A��
	void tutorial();

private:
	//�X�J�C�h�[���̉�]
	float skyShpereRotY = 0.0002f;

	//���I���\�[�X
	std::unique_ptr<Model> rubble01;
	std::unique_ptr<Model> rubble02;

	const int maxRubbleNum = 20;

	std::list<std::unique_ptr<object3dFBX>> rubbles_1;
	std::list<std::unique_ptr<object3dFBX>> rubbles_2;

	//���o�p�J����
	std::unique_ptr<Camera> stagingCamera;

	//�J�E���g�_�E���E�X�^�[�g�A�C�R��
	SingleSprite countDownSprite[3];
	SingleSprite playStart;

	//�v���C�V�[���̎���
	int totalPlayFlameCount = 0;

	//���x���[�^�[
	SingleSprite heightGauge;

	//�v���C���[�̍��x
	SingleSprite playerHeight;
	SingleSprite playerHeightIcon;

	int targetnum = 0;

	//�J�E���g�_�E���̃C�[�W���O�t���O
	bool isCountDown = false;
	bool isStartIcon = false;
	int countDownNum = 0;
	int startIconTime = 0;
	int countDownTime = 60;
	easingManager countDownEase;

	//�`���[�g���A���p�e�L�X�g
	SingleSprite moveText;
	bool isMoveText = false;
	SingleSprite boostText;
	bool isBoostText = false;
	SingleSprite shotText;
	bool isShotText = false;
	SingleSprite missileText;
	bool isMissileText = false;
	SingleSprite shootingText;
	bool isShootingText = false;

	//�{�X�o���t���O
	bool isBoss = false;
};