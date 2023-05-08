#pragma once
#include"../Base/WindowGenerate.h"
#include"../Base/DirectX_Base.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../Audio/Audio.h"
#include"../2D/Debug_Text.h"
#include"../camera/Camera.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"
#include"../staging/Easing.h"
#include"../Player/Player.h"
#include"../Player/Enemy.h"
#include"../Player/Boss.h"
#include"../Player/uniteBoss.h"
#include"../Player/CheckHItManager.h"
#include<random>
#include<time.h>
#include<memory>
#include<list>

enum class sceneType
{
	title = 0,
	select = 1,
	play = 2,
	result = 3
};

class primitiveScene
{
public:
	static void setStaticData(directX* Directx, dxinput* Input, Audio* Audio);

	//���\�[�X�̓ǂݍ���
	virtual void loadResources() = 0;

	//������
	virtual void initialize() = 0;

	//�p�����[�^�̃��Z�b�g
	virtual void setParameter() = 0;

	//�X�V
	virtual void updata() = 0;

	//�`��(3D)
	virtual void draw3D() = 0;

	//�`��(2D)
	virtual void draw2D() = 0;

	//���̃V�[���Ɉڂ邩�ǂ���
	bool getIsNextScene() { return isNextScene; }

public:
	//main.cpp����Q�Ƃ���̂Ő��|�C���^
	static directX* directx;
	static dxinput* input;
	static Audio* audio;

	//���̃V�[���Ɉڍs����t���O
	bool isNextScene = false;

	//-------�����̃V�[�����܂����Ŏg�����͂����ɏ���-------
	//���C�g
	static Light* light;
	//�G���X�g
	static std::list<std::unique_ptr<Enemy>> enemyList;
	//�ʏ�{�X
	static std::unique_ptr<Boss> normalBoss;
	//���j�b�g�{�X
	static std::unique_ptr<uniteBoss> UniteBoss;
	//�v���C���[
	static std::unique_ptr<Player> playerPointer;
	//�E�F�[�u�\���X�v���C�g
	static std::unique_ptr<SingleSprite> enemyWaveBar;
	static std::unique_ptr<SingleSprite> playerWaveIcon;
	static std::list<std::unique_ptr<SingleSprite>> enemyWaveIcons;
	const float totalWaveBarLength = 500;
	const float waveBarPosX = 1180;
	//�X�e�[�W�ԍ�
	static int stageNum;
	//�ő�X�e�[�W�ԍ�
	static const int maxStageNum = 3;
	//���݂̃X�e�[�W���x��(�G��S�ł������1�オ��)
	static int nowStageLevel;
	//�X�e�[�W���Ƃ̍ő僌�x��(�����܂œ��B����ƃ{�X�o��)
	static int maxStageLevel;
	//�`���[�g���A��
	static bool isTutorial;
	//�N���A���I�[�o�[��(true:�N���A/false:�I�[�o�[)
	static bool isClearOrOver;
	//�A�C�R�����m�̋���
	static float nextWaveDis;
};