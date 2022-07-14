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

//�T�E���h�����ǂݍ��݊֐�
void GameScene::Load_sounds()
{

}

//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)������
void GameScene::Load_Sprites()
{
	sample_back.size = { 1280,720 };
	sample_back.GenerateSprite("sample_back.jpg");
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

	//���ǂݍ���
	Load_sounds();

	camera = new Camera();

	camera->SetTarget({ 0, 2.5f, 0 });
	camera->SetEye({ -10,5,0 });

	//�X�v���C�g�N���X������
	SingleSprite::SetStaticData(directx->dev.Get());

	//�f�o�b�O�e�L�X�g������
	debugtext.Init();

	//�X�v���C�g����
	Load_Sprites();

	//3d�I�u�W�F�N�g����
	Object3d_FBX::SetDevice(directx->dev.Get());

	Object3d_FBX::SetCamera(camera);

	Object3d_FBX::CreateGraphicsPipeline();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");
	SkyModel = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");

	object = new Object3d_FBX;
	object->Initialize();
	object->SetModel(model);
	//object->PlayAnimation();


	followcamera = new FollowCamera();

	

	SkySphere = new Object3d_FBX;
	SkySphere->Initialize();
	SkySphere->SetModel(SkyModel);
	SkySphere->SetScale({ 5.0f,5.0f,5.0f });
}

//�f�o�b�O�e�L�X�g
void GameScene::debugs_print()
{
}

//�^�C�g����ʍX�V
void GameScene::Title_update()
{
	object->Update();
	SkySphere->Update();
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
	SkySphere->Draw(directx->cmdList.Get());
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

	sample_back.SpriteUpdate();

	camera->Update();
}

void GameScene::DrawBack()
{
	//�w�i�`��
	sample_back.DrawSprite(directx->cmdList.Get());

	//�[�x�o�b�t�@�N���A
	//directx->depthclear();
}

//�`��
void GameScene::Draw3D()
{
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
