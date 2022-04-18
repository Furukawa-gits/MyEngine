#include"GameScene.h"
#include<cassert>

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

//�e�N�X�`���ǂݍ��݂����֐�
void GameScene::Load_textures()
{
	texture.LoadTexture(1, L"Resources/Image/sample_back.jpg", directx->dev.Get());
	texture.LoadTexture(2, L"Resources/Ball.png", directx->dev.Get());
}

//�T�E���h�����ǂݍ��݊֐�
void GameScene::Load_sounds()
{
	
}

//�X�v���C�g(�e�N���X�Ɉˑ����Ȃ����)������
void GameScene::Load_Sprites()
{
	sample_back.texnumber = 1;
	sample_back.GenerateSprite(directx->dev.Get(), win_width, win_hight, sample_back.texnumber, &texture);
	sample_back.anchorpoint = { 0,0 };
	sample_back.size = { 1280,720 };
	sample_back.texSize = { 1280,720 };
	sample_back.SpriteTransferVertexBuffer(&texture, false);

	BallSprite.texnumber = 2;
	BallSprite.GenerateSprite(directx->dev.Get(), win_width, win_hight, BallSprite.texnumber, &texture);
	BallSprite.texSize = { 1400,1400 };
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

	object3D::SetStaticData(camera, this->directx, &object3dcommon);

	//�e�N�X�`��������
	texture.Init(directx->dev.Get());

	//�f�o�b�O�e�L�X�g�e�N�X�`���ǂݍ���
	texture.LoadTexture(debugTextnum, L"Resources/Image/Debug_Text.png", directx->dev.Get());

	//�f�o�b�O�e�L�X�g������
	debugtext.Init(directx->dev.Get(), win_width, win_hight, debugTextnum, &texture);

	//�e�N�X�`���ǂݍ���
	Load_textures();

	//�X�v���C�g���ʃf�[�^����
	spritecommon.SpriteCommonCreate(directx->dev.Get(), win_width, win_hight);

	//�X�v���C�g������
	Load_Sprites();

	ball.Pos = { 100,400,0 };
	BallSprite.SpriteTransferVertexBuffer(&texture, false);
}

//�f�o�b�O�e�L�X�g
void GameScene::debugs_print()
{
	char text[20] = "SPACE:Start";
	char text_1[20] = "R:Reset";
	debugtext.Print(spritecommon, &texture, text, 50, 50);
	debugtext.Print(spritecommon, &texture, text_1, 50, 70);
}

//�^�C�g����ʍX�V
void GameScene::Title_update()
{
	if (input->Triger(DIK_R))
	{
		ball.Pos = { 100,400,0 };
		ball.IsMove = false;
	}

	if (input->Triger(DIK_SPACE))
	{
		ball.Set({ 100,400,0 }, { 25,-20,0 });
	}

	ball.Update();

	BallSprite.position = ball.Pos;
	BallSprite.SpriteUpdate(spritecommon);

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
	BallSprite.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());
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
	spritecommon.SpriteCommonBeginDraw(directx->cmdList.Get(), &texture);
	sample_back.DrawSprite(directx->cmdList.Get(), &texture, directx->dev.Get());

	//�[�x�o�b�t�@�N���A
	directx->depthclear();

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

	debugtext.DrawAll(directx->cmdList.Get(), spritecommon, &texture, directx->dev.Get());
}
