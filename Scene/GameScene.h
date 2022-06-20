#pragma once
#include"../Base/WindowGenerate.h"
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
#include"../FbxLoder/Object3d_FBX.h"

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

	//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)

	//3d�I�u�W�F�N�g
	Model* model = nullptr;
	Object3d_FBX* object = nullptr;

	//�w�i
	SingleSprite sample_back;

	//�Q�[������
	int game_time = 0;

	//�V�[��
	//scene scene = scene::title;
	int scene = title;

	//�E�F�[�u
	bool Ischangescene = false;

	bool Isclose = false;

	//���U���g�̕\���؂�ւ�
	int result_count = 0;
};