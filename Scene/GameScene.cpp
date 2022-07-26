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

	BallSprite.anchorpoint = { 0.5f,0.5f };
	BallSprite.size = { 100,100 };
	BallSprite.GenerateSprite("Ball.png");

	BallSprite2.anchorpoint = { 0.5f,0.5f };
	BallSprite2.size = { 100,100 };
	BallSprite2.GenerateSprite("Ball.png");
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


	ball.Pos = { 200,-400,0 };
	ball.Radius = { 50,50,50 };
	ball.IsSlide = true;

	ball2.Pos = { 900,-400,0 };
	ball2.Radius = { 50,50,50 };
	ball2.M = 3.0f;
	ball2.IsSlide = true;

	//3d�I�u�W�F�N�g����

}

//�f�o�b�O�e�L�X�g
void GameScene::debugs_print()
{
	debugtext.Print("SPACE:Start", 50, 50);
	debugtext.Print("R:Reset", 50, 70);

	char text1[30];
	snprintf(text1, sizeof(text1), "ball:Accel=%f", ball.Accel.x);
	debugtext.Print(text1, 50, 90);
	debugtext.Print("ball:M=1.0", 50, 110);

	char text2[30];
	snprintf(text2, sizeof(text2), "ball2:Accel=%f", ball2.Accel.x);
	debugtext.Print(text2, 1000, 90);
	debugtext.Print("ball2:M=3.0", 1000, 110);
}

//�^�C�g����ʍX�V
void GameScene::Title_update()
{
	if (input->Triger(DIK_R))
	{
		ball.Pos = { 200,-400,0 };
		ball.IsMove = false;
		ball2.Pos = { 900,-400,0 };
		ball2.IsMove = false;
	}

	if (input->Triger(DIK_SPACE))
	{
		ball.Set({ 200,-400,0 }, { 50,0,0 });
		//ball2.Set({ 900,-400,0 }, { -50,0,0 });
	}

	ball.Update(1.2f);
	ball2.Update(1.5f);

	ball.hitBallSlide(ball2);

	BallSprite.position = { ball.Pos.x,ball.Pos.y * -1,ball.Pos.z };
	BallSprite.SpriteUpdate();

	BallSprite2.position = { ball2.Pos.x,ball2.Pos.y * -1,ball2.Pos.z };
	BallSprite2.SpriteUpdate();

	debugs_print();
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
	BallSprite.DrawSprite(directx->cmdList.Get());
	BallSprite2.DrawSprite(directx->cmdList.Get());

	//object->Draw(directx->cmdList.Get());

	//cameraobj->Draw(directx->cmdList.Get());
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

	debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::DrawSP()
{
	debugtext.DrawAll(directx->cmdList.Get());
}
