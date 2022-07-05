#include"GameScene.h"
#include"../FbxLoder/Object3d_FBX.h"
#include<cassert>

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete(object);
	delete(model);
}

//�e�N�X�`���ǂݍ��݂����֐�
void GameScene::Load_textures()
{
	texture.LoadTexture(1, L"Resources/Image/sample_back.jpg", directx->dev.Get());
}

//�T�E���h�����ǂݍ��݊֐�
void GameScene::Load_sounds()
{

}

//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)������
void GameScene::Load_Sprites()
{
	//sample_back.SetPipelineStateSprite(directx->dev.Get());
	sample_back.size = { 1280,720 };
	sample_back.GenerateSprite(directx->dev.Get(),1, &texture);
}

//������
void GameScene::Init(directX* directx, dxinput* input, Audio* audio)
{
	//null�`�F�b�N
	assert(directx);
	assert(input);
	assert(audio);

	//������������N
	this->directx = directx;
	this->input = input;
	this->audio = audio;

	Load_sounds();

	//3d�I�u�W�F�N�g���ʃf�[�^����
	object3dcommon.init(directx);

	camera = new Camera(1280, 720);

	camera->SetTarget({ 0, 2.5f, 0 });
	camera->SetEye({ -10,5,0 });

	//�e�N�X�`��������
	texture.Init(directx->dev.Get());

	//�f�o�b�O�e�L�X�g�e�N�X�`���ǂݍ���
	texture.LoadTexture(debugTextnum, L"Resources/Image/Debug_Text.png", directx->dev.Get());
	texture.LoadTexture(2, L"Resources/white1x1.png", directx->dev.Get());

	//�f�o�b�O�e�L�X�g������
	debugtext.Init(directx->dev.Get(), win_width, win_hight, debugTextnum, &texture);

	//�e�N�X�`���ǂݍ���
	Load_textures();

	//�X�v���C�g���ʃf�[�^����
	spritecommon.SpriteCommonCreate(directx->dev.Get(), win_width, win_hight);

	//�X�v���C�g������
	Load_Sprites();

	//3d�I�u�W�F�N�g����
	Object3d_FBX::SetDevice(directx->dev.Get());

	Object3d_FBX::SetCamera(camera);

	Object3d_FBX::CreateGraphicsPipeline();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");

	object = new Object3d_FBX;
	object->Initialize();
	object->SetModel(model);
	object->PlayAnimation();
}

//�f�o�b�O�e�L�X�g
void GameScene::debugs_print()
{
}

//�^�C�g����ʍX�V
void GameScene::Title_update()
{
	object->Update();
}

//�v���C��ʍX�V
void GameScene::Play_update()
{

}

//���U���g��ʍX�V
void GameScene::Result_update()
{

}

//�^�C�g����ʕ`��
void GameScene::Title_draw()
{
	object->Draw(directx->cmdList.Get());
}

//�v���C��ʕ`��
void GameScene::Play_draw()
{

}

//���U���g��ʕ`��
void GameScene::Result_draw()
{
}


//�X�V
void GameScene::Update()
{
	//�}�E�X���W�X�V
	MOUSE_POS = { (float)input->mouse_p.x,(float)input->mouse_p.y,0.0f };

	//�Q�[�����ԃJ�E���g
	game_time++;

	//�V�[���؂�ւ�

	//�^�C�g�����
	if (scene == title)
	{
		Title_update();
	}

	//�v���C���
	if (scene == play)
	{
		Play_update();
	}

	//�N���A���
	if (scene == clear || scene == over)
	{
		Result_update();
	}

	sample_back.SpriteUpdate(spritecommon);

	camera->Update();
}

//�`��
void GameScene::Draw()
{
	//�w�i�`��
	//spritecommon.SpriteCommonBeginDraw(directx->cmdList.Get(), &texture);
	//sample_back.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());

	//�[�x�o�b�t�@�N���A
	//directx->depthclear();

	if (scene == title)
	{
		Title_draw();
	}
	else if (scene == play)
	{
		Play_draw();
	}
	else
	{
		Result_draw();
	}
}
