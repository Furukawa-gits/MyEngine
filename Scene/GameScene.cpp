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

	strings::setInput(input);

	for (int i = 0; i < 5; i++)
	{
		objs[i].init();
	}

	objs[0].stat = 0;
	objs[0].pos = { 640,100 };

	for (int i = 1; i < 5; i++)
	{
		objs[i].stat = 1;
		objs[i].vel = { 0,0 };
		objs[i].pos = objs[i - 1].pos;
		objs[i].link0 = &objs[i - 1];
		objs[i - 1].link1 = &objs[i];
	}

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
	objs[0].grabbed = 1;
	for (int i = 0; i < 5; i++)
	{
		objs[i].update();
	}
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
	for (int i = 0; i < 5; i++)
	{
		objs[i].disp(directx->cmdList.Get());
	}
}
