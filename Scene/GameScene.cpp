#include"GameScene.h"
#include"../FbxLoder/Object3d_FBX.h"
#include<cassert>

GameScene::GameScene()
{
}

GameScene::~GameScene()
{

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

	BallSprite1.anchorpoint = { 0.5f,0.5f };
	BallSprite1.position = { 100,160,0 };
	BallSprite1.size = { 50,50 };
	BallSprite1.GenerateSprite("Ball.png");

	BallSprite2.anchorpoint = { 0.5f,0.5f };
	BallSprite2.position = { 100,360,0 };
	BallSprite2.size = { 50,50 };
	BallSprite2.GenerateSprite("Ball.png");

	BallSprite3.anchorpoint = { 0.5f,0.5f };
	BallSprite3.position = { 100,560,0 };
	BallSprite3.size = { 50,50 };
	BallSprite3.GenerateSprite("Ball.png");
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
	camera->SetEye({ 0,5,-50 });
	//�X�v���C�g�N���X������
	SingleSprite::SetStaticData(directx->dev.Get());

	//�f�o�b�O�e�L�X�g������
	debugtext.Init();

	//�X�v���C�g����
	Load_Sprites();

	//3d�I�u�W�F�N�g����

}

//�f�o�b�O�e�L�X�g
void GameScene::debugs_print()
{
	debugtext.Print("SPACE:Start", 50, 50);
	debugtext.Print("R:Reset", 50, 70);

	char text1[30];
	//snprintf(text1, sizeof(text1), "ball:Accel=%f", ball.Accel.x);
	debugtext.Print(text1, 50, 90);
	debugtext.Print("ball:M=1.0", 50, 110);
	//snprintf(text1, sizeof(text1), "ball:Repulsion=%f", ball.repulsion);
	debugtext.Print(text1, 50, 130);
}

//�^�C�g����ʍX�V
void GameScene::Title_update()
{
	//���Z�b�g
	if (input->Triger(DIK_R))
	{
		in.isStart = false;
		out.isStart = false;
		inOut.isStart = false;
	}

	//�X�^�[�g
	if (input->Triger(DIK_SPACE))
	{
		in.easingSet();
		out.easingSet();
		inOut.easingSet();
	}

	//�e�X�v���C�g��X���W���C�[�W���O
	BallSprite1.position.x = in.easeInQuad(100, 900, 60);
	BallSprite2.position.x = out.easeOutQuad(100, 900, 60);
	BallSprite3.position.x = inOut.easeInOutQuad(100, 900, 60);

	BallSprite1.SpriteUpdate();
	BallSprite2.SpriteUpdate();
	BallSprite3.SpriteUpdate();
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

	//�C�[�W���O�p�t���[���J�E���g�X�V
	easing::easingUpdate();

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

	debugs_print();
}

void GameScene::DrawBack()
{
	//�w�i�X�v���C�g�`��
	//sample_back.DrawSprite(directx->cmdList.Get());
}

//�`��
void GameScene::Draw3D()
{
	//�Q�[�����V�[�����Ƃ̕`��
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

	//debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::DrawSP()
{
	BallSprite1.DrawSprite(directx->cmdList.Get());
	BallSprite2.DrawSprite(directx->cmdList.Get());
	BallSprite3.DrawSprite(directx->cmdList.Get());
}
