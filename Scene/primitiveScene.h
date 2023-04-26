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
	virtual void loadResources();

	//������
	virtual void initialize();

	//�p�����[�^�̃��Z�b�g
	virtual void setParameter();

	//�X�V
	virtual void updata();

	//�`��(3D)
	virtual void draw3D();

	//�`��(2D)
	virtual void draw2D();

	//���̃V�[���Ɉڂ邩�ǂ���
	bool getIsNextScene() { return isNextScene; }

public:
	//main.cpp����Q�Ƃ���̂Ő��|�C���^
	static directX* directx;
	static dxinput* input;
	static Audio* audio;

	//���̃V�[���Ɉڍs����t���O
	bool isNextScene = false;

	//�����̃V�[�����܂����Ŏg�����͂����ɏ���
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
};