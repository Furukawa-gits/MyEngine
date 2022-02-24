#pragma once
#include"../Base/Window_Generate.h"
#include"../Base/DirectX_Base.h"
#include"../Base/TexManager.h"
#include"../2D/SpriteCommon.h"
#include"../2D/SpriteSingleunit.h"
#include"../Input/dxInput.h"
#include"../3D/3Dobject.h"
#include"../Audio/Audio.h"
#include"../2D/Debug_Text.h"
#include"../camera/Camera.h"
#include"../Player/Player.h"

enum scene
{
	title = 0,
	play = 1,
	over = 2,
	clear = 3
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

//�E�F�[�u���Ƃ̓G�̐�
const int ENEMY_NUM_1 = 8;

const int ENEMY_NUM_2 = 8;

const int ENEMY_NUM_3 = 20;

class GameScene
{
public:
	//�R���X�g���N�^
	GameScene();

	//�f�X�g���N�^
	~GameScene();

	//�e�N�X�`���ǂݍ��݂����֐�
	void Load_textures();

	//���ǂݍ��݂����֐�
	void Load_sounds();

	//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)������
	void Load_Sprites();

	//������
	void Init(directX* directx, dxinput* input, Audio* audio);

	//�f�o�b�O�e�L�X�g�X�V
	void debugs_print();

	//�^�C�g����ʃA�j���[�V����
	void Title_update();

	//�v���C��ʍX�V
	void Play_update();

	//���U���g��ʍX�V
	void Result_update();

	//�V�[�����Ƃ̕`��
	void Title_draw();
	void Play_draw();
	void Result_draw();

	//�C�[�Y�C��
	float easeIn(const float startX, const float endX, const float time)
	{
		float y = time * time;
		return startX * (1.0f - y) + endX * y;
	}

	//�C�[�Y�A�E�g
	float easeOut(const float startX, const float endX, const float time)
	{
		float y = time * (2 - time);
		return startX * (1.0f - y) + endX * y;
	}

	//�X�V
	void Update();

	//�`��
	void Draw();

public:

	static const int debugTextnum = 0;
	//�����
	directX* directx = nullptr;
	dxinput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugtext;
	XMFLOAT3 MOUSE_POS;
	Camera* camera = nullptr;

	/// <summary>
	/// �Q�[���Ɏg���ϐ����͂�����
	/// </summary>
	SpriteCommon spritecommon;				//�X�v���C�g���ʃf�[�^
	ObjectCommon object3dcommon;	//3d�I�u�W�F�N�g���ʃf�[�^
	TexManager texture;						//�e�N�X�`��

	//��
	SoundData Title_bgm;

	//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)
	SingleSprite title_sp;
	SingleSprite title_button;
	SingleSprite nextbutton;
	SingleSprite closebutton;
	SingleSprite clear_sp;
	SingleSprite gameover_sp;

	SingleSprite move_ui;
	SingleSprite shot_ui;
	SingleSprite missile_ui;

	//3d�I�u�W�F�N�g
	Player player;

	SingleSprite sample_back;

#pragma region �G�̏����ʒu�C���f�b�N�X
	XMFLOAT3 enemy_pos_First_Second[8] = {
		{ -50.0f, 40.0f,400.0f },
		{ -25.0f, 40.0f,400.0f },
		{  25.0f, 40.0f,400.0f },
		{  50.0f, 40.0f,400.0f },
		{ -50.0f,-40.0f,400.0f },
		{ -25.0f,-40.0f,400.0f },
		{  25.0f,-40.0f,400.0f },
		{  50.0f,-40.0f,400.0f }
	};

	XMFLOAT3 enemy_pos_Third[20] =
	{
		{-100, 70,400},
		{ -80, 60,400},
		{ -60, 50,400},
		{ -40, 40,400},
		{ -20, 30,400},

		{ 100, 70,400},
		{  80, 60,400},
		{  60, 50,400},
		{  40, 40,400},
		{  20, 30,400},

		{-100,-70,400},
		{ -80,-60,400},
		{ -60,-50,400},
		{ -40,-40,400},
		{ -20,-30,400},

		{ 100,-70,400},
		{  80,-60,400},
		{  60,-50,400},
		{  40,-40,400},
		{  20,-30,400}
	};
#pragma endregion

	Enemy enemy_1[ENEMY_NUM_1];

	Enemy enemy_2[ENEMY_NUM_2];

	Enemy enemy_3[ENEMY_NUM_3];

	//�Q�[������
	int game_time = 0;

	//�V�[��
	//scene scene = scene::title;
	int scene = title;

	//�E�F�[�u
	//wave wave = wave::first;

	int wave = 1;

	bool Ischangescene = false;

	bool Isclose = false;

	//�����c���Ă���G�̐�
	int enemyarivenum = 0;


	//���U���g�̕\���؂�ւ�
	int result_count = 0;

	//�t�F�[�h�̃p�^�[��
	int fade_patern;

	//�V�[���`�F���W�t���O
	bool IsSceneChange = false;

	//�V�[�����܂����t���O
	bool Istitle_to_play = false;
	bool IsPlay_to_clear = false;
	bool IsPlay_to_over = false;
	bool IsClear_to_title = false;
	bool IsOver_to_title = false;

	//�E�F�[�u�`�F���W�t���O
	bool IsWaveChange = false;

	//�K�C�h�\���t���O
	bool IsGuide = false;

	//�V�[���J�ڗp(�C�[�Y�C���A�E�g)
	float start_X = -1280.0f;
	float end_X = 0.0f;
	float end2_X = 1280.0f;

	float start_Y = -720.0f;
	float end_Y = 0.0f;
	float end2_Y = 720.0f;

	float maxTime = 1.0f;
	float timeRate = 0.0f;
	float addTime = 0.1f;

	float timeRate2 = 0.0f;
	float addTime2 = 0.1f;

	bool Isout = false;
	bool Isin = false;
};