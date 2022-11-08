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

#include<random>
#include<time.h>

enum class sceneType
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
//const int ENEMY_NUM_1 = 8;
//
//const int ENEMY_NUM_2 = 8;
//
//const int ENEMY_NUM_3 = 20;

class GameScene
{
public:
	//�R���X�g���N�^
	GameScene();

	//�f�X�g���N�^
	~GameScene();

	//���ǂݍ��݂����֐�
	void Load_sounds();

	//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)������
	void Load_Sprites();

	//������
	void Init(directX* directx, dxinput* input, Audio* audio);

	//�f�o�b�O�e�L�X�g�X�V
	void debugs_print();

	//�V�[�����Ƃ̍X�V
	void Title_update();
	void Play_update();
	void Result_update();

	//�V�[�����Ƃ̕`��
	void Title_draw();
	void Play_draw();
	void Result_draw();

	//�X�V
	void Update();

	//�`��
	void DrawBack();
	void Draw3D();
	void Draw2D();

	//�z�[�~���O�^�[�Q�b�g�̃Z�b�e�B���O
	void checkHitPlayerTarget();

	bool Isclose = false;

private:

	//-------------�Q�[���Ɏg���ϐ����͂�����聫-------------
	static const int debugTextnum = 0;

#pragma region �����
	directX* directx = nullptr;
	dxinput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugtext;
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	//Camera* camera = nullptr;
#pragma endregion

	//��]��
	XMFLOAT3 objectRot = { 0,0,0 };

	float up = 0.0f;
	float right = 0.0f;

	float pitch = 0.0f;
	float yow = 0.0f;
	float roll = 0.0f;
	//��

	//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)

	//3d�I�u�W�F�N�g
	Model* model = nullptr;
	Model* SkyModel = nullptr;
	Object3d_FBX* object = nullptr;
	Object3d_FBX* skySphere = nullptr;
	Object3d_FBX* cameraobj = nullptr;

	//�v���C���[
	Player testPlayer;

	int targetnum = 0;

	//�G(test)
	static const int maxEnemyNum = 20;
	Enemy testEnemys[maxEnemyNum];
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

	//�{�X�o���t���O
	bool isBoss = false;

	//�Ə��X�v���C�g�֘A
	int mousePressCount = 0;
	SingleSprite target;
	bool isTarget = false;

	//�w�i
	SingleSprite sample_back;

	//�Q�[������
	int game_time = 0;

	//�V�[��
	sceneType scene = sceneType::title;

	//�E�F�[�u
	bool Ischangescene = false;

	//���U���g�̕\���؂�ւ�
	int result_count = 0;
};