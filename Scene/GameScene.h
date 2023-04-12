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
#include"../Player/uniteBoss.h"
#include"../Player/CheckHItManager.h"

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

	//�`���[�g���A��
	void tutorial();

	//�X�e�[�W�ǂݍ���
	bool loadStage();

	//�V�[�����Ƃ̕`��
	void TitleDraw3d();
	void TitleDraw2d();

	void SelectDraw3d();
	void SelectDraw2d();

	void PlayDraw3d();
	void PlayDraw2d();

	void ResultDraw3d();
	void ResultDraw2d();

	//�E�F�[�u�o�[�`��
	void drawNowWave();

	//�~�j�}�b�v�`��
	void drawPositionUI();

	//�z�[�~���O�^�[�Q�b�g�̃Z�b�e�B���O
	void checkHitPlayerTarget();

	//�J�E���g�_�E��
	void countDown();

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
	//��

	//3d�I�u�W�F�N�g
	Model* model = nullptr;
	Model* SkyModel = nullptr;
	Model* groundModel = nullptr;
	Object3d_FBX* object = nullptr;
	Object3d_FBX* skySphere = nullptr;
	Object3d_FBX* ground = nullptr;
	Object3d_FBX* cameraobj = nullptr;

	//�X�J�C�h�[���̉�]
	float skyShpereRotY = 0.0002f;

	//���C�g
	Light* light = nullptr;

	Camera* stagingCamera = nullptr;

	//�v���C���[
	std::unique_ptr<Player> player_p;

	int targetnum = 0;

	//�G(test)
	static const int maxEnemyNum = 10;
	std::list<std::unique_ptr<Enemy>> enemyList;
	bool isEnemySimple = false;

	/// <summary>
	/// �e�X�g�p�{�X
	/// <para>��������������HP��30�ɐݒ�</para>
	/// <para>�A�b�v�f�[�g�������œG���S�ł���܂ő҂�</para>
	/// <para>�S�ł�����o��</para>
	/// <para>HP���؂��������</para>
	/// �I�u�W�F�N�g�̃T�C�Y�͑傫�����ĕ�����₷��
	/// </summary>
	std::unique_ptr<Boss> testBoss;

	std::unique_ptr<uniteBoss> testUniteBoss;

	//�{�X��hp�Q�[�W
	std::vector<std::unique_ptr<SingleSprite>> bossHitPoints;

	//�{�X�o���t���O
	bool isBoss = false;

	//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)
	//�w�i
	std::unique_ptr<SingleSprite> sample_back;
	//�^�C�g��
	SingleSprite gameTitle;
	//�X�^�[�g�{�^��
	SingleSprite startButton;
	//�X�e�[�W�A�C�R��
	SingleSprite stage[5];
	//�Z���N�g�A�C�R��
	SingleSprite selects[3];
	float selectIconSizeX = 150;
	//�J�E���g�_�E���E�X�^�[�g�A�C�R��
	SingleSprite countDownSprite[3];
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
	//�`���[�g���A���U��
	SingleSprite toTutorial;
	//�v���C�V�[���̎���
	int totalPlayFlameCount = 0;
	//�~�j�}�b�v
	SingleSprite miniMap;
	//���x���[�^�[
	SingleSprite heightGauge;
	//�v���C���[�̍��x
	SingleSprite playerHeight;
	SingleSprite playerHeightIcon;

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

	//�ő�X�e�[�W�ԍ�
	const int maxStageNum = 3;

	//���݂̃X�e�[�W���x��(�G��S�ł������1�オ��)
	int nowStageLevel = 1;

	SingleSprite enemyWaveBar;
	SingleSprite playerWaveIcon;
	std::list<std::unique_ptr<SingleSprite>> enemyWaveIcons;

	const float totalWaveBarLength = 500;
	const float waveBarPosX = 1180;
	float nextWaveDis = 0;

	//�X�e�[�W���Ƃ̍ő僌�x��(�����܂œ��B����ƃ{�X�o��)
	int stageLevel = 1;

	//�G�̗N����
	int enemySpawnNum = 0;

	// stageLevel = stageNum + 2
	// stageLevel ���ő�܂ōs���ƃ{�X�o��
	// �{�X�� HP �� stageLevel + 5
	// �{�X��|���ƃN���A

	//�X�e�[�W�A�C�R���̃C�[�W���O
	easingManager stageIconEase;

	//�X�e�[�W�A�C�R���𓮂����Ă��邩�ǂ���
	bool isMoveStageIcon = false;

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

	//���ǂ����̃{�^����I��ł���̂�
	//-1�Fselect , 1�Ftitle
	int isSelectOrTitle = -1;
	bool isMoveSelectIcon = false;

	bool isPushTitle = false;

	//�`���[�g���A��
	bool isTutorial = false;

	bool isLoadStage = false;
};