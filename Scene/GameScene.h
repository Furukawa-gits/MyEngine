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

#include"../Player/Player.h"
#include"../Player/Enemy.h"
#include"../Player/Boss.h"

#include"../staging/Easing.h"

#include<random>
#include<time.h>

enum class sceneType
{
	title = 0,
	select = 1,
	play = 2,
	over = 3,
	clear = 4
};

enum ChangePat
{
	X,
	Y
};

enum class wave
{
	first = 0,
	second = 1,
	third = 2
};

class GameScene
{
public:
	//�R���X�g���N�^
	GameScene();

	//�f�X�g���N�^
	~GameScene();

	//������
	void Init(directX* directx, dxinput* input, Audio* audio);

	//�X�V
	void Updata();

	//�`��
	void DrawBack();
	void Draw3D();
	void Draw2D();

	bool Isclose = false;

private:

	//�V�[�����Ƃ̍X�V
	void Title_updata();
	void Select_updata();
	void Play_updata();
	void Result_updata();

	//�V�[�����Ƃ̕`��
	void Title_draw();
	void Select_draw();
	void Play_draw();
	void Result_draw();

	//�z�[�~���O�^�[�Q�b�g�̃Z�b�e�B���O
	void checkHitPlayerTarget();

	//���ǂݍ��݂����֐�
	void Load_sounds();

	//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)������
	void Load_Sprites();

	//�f�o�b�O�e�L�X�g�X�V
	void debugs_print();

	//-------------�Q�[���Ɏg���ϐ����͂�����聫-------------
	static const int debugTextnum = 0;

#pragma region �����
	directX* directx = nullptr;
	dxinput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugtext;
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
#pragma endregion

	//��]��
	XMFLOAT3 objectRot = { 0,0,0 };

	float up = 0.0f;
	float right = 0.0f;

	float pitch = 0.0f;
	float yow = 0.0f;
	float roll = 0.0f;
	//��

	//3d�I�u�W�F�N�g
	Model* model = nullptr;
	Model* SkyModel = nullptr;
	Object3d_FBX* object = nullptr;
	Object3d_FBX* skySphere = nullptr;
	Object3d_FBX* cameraobj = nullptr;

	//�v���C���[
	Player player;

	int targetnum = 0;

	//�G(test)
	static const int maxEnemyNum = 20;
	Enemy testEnemys[maxEnemyNum];
	std::vector<Enemy> Enemys;
	bool isEnemySimple = false;

	/// <summary>
	/// �e�X�g�p�{�X
	/// <para>��������������HP��30�ɐݒ�</para>
	/// <para>�A�b�v�f�[�g�������œG���S�ł���܂ő҂�</para>
	/// <para>�S�ł�����o��</para>
	/// <para>HP���؂��������</para>
	/// �I�u�W�F�N�g�̃T�C�Y�͑傫�����ĕ�����₷��
	/// </summary>
	Boss testBoss;

	//�{�X��hp�Q�[�W
	SingleSprite bossHp[5];

	//�{�X�o���t���O
	bool isBoss = false;

	//�Ə��X�v���C�g�֘A
	int mousePressCount = 0;
	SingleSprite target;
	bool isTarget = false;

	//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)
	//�w�i
	SingleSprite sample_back;

	//�^�C�g��
	SingleSprite gameTitle;

	//�X�^�[�g�{�^��
	SingleSprite startButton;

	//�X�e�[�W�A�C�R��
	SingleSprite stage1;
	SingleSprite stage2;

	//�J�E���g�_�E���E�X�^�[�g�A�C�R��
	SingleSprite countDown[3];
	SingleSprite playStart;

	//���U���g���
	SingleSprite resultScreen[2];

	//�N���A���
	SingleSprite clearText;

	//�Q�[���I�[�o�[���
	SingleSprite overText;

	//�^�C�g���E�Z���N�g�{�^��
	SingleSprite titleButton;
	SingleSprite selectButton;

	//�Q�[������
	int game_time = 0;

	//�V�[���ԍ�
	sceneType scene = sceneType::title;

	//�^�C�g�����o�t���O
	bool isTitleStage = false;

	//�X�^�[�g�{�^���̃C�[�W���O
	easingManager startButtonEase_y;
	easingManager startButtonEase_x;

	bool isPushStart = false;

	//�V�[���`�F���W�t���O
	bool Ischangescene = false;

	//�X�e�[�W�ԍ�
	int stageNum = 1;

	//�X�e�[�W�A�C�R���̃C�[�W���O
	easingManager stageIconEase;

	//�X�e�[�W�A�C�R���𓮂����Ă��邩�ǂ���
	bool isMoveStageIcon = false;

	//�J�E���g�_�E���̃C�[�W���O�t���O
	bool isCountDown = false;
	bool isStartIcon = false;
	int countDownNum = 0;
	int startIconTime = 0;
	easingManager countDownEase;
	easingManager startEase;

	//���U���g��ʂ̃C�[�W���O�t���O
	bool isMoveScreen = false;
	bool isScreenEase = false;
	bool isTextEase = false;

	easingManager resultScreenEase;
	easingManager clearTextEase;
	easingManager overTextEase;
};