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
	
	//�X�v���C�g�N���X������
	SingleSprite::SetStaticData(directx->dev.Get());

	//�f�o�b�O�e�L�X�g������
	debugtext.Init();

	//�X�v���C�g����
	Load_Sprites();

	//3d�I�u�W�F�N�g����
	Object3d_FBX::SetDevice(directx->dev.Get());

	Object3d_FBX::CreateGraphicsPipeline();

	Object3d_FBX::CreateGraphicsPipelineSimple();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");
	SkyModel = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");

	//�v���C���[������
	testPlayer.init(input, directx);

	skySphere = new Object3d_FBX;
	skySphere->Initialize();
	skySphere->SetModel(SkyModel);
	skySphere->SetScale({ 5.0f,5.0f,5.0f });

	cameraobj = new Object3d_FBX;
	cameraobj->Initialize();
	cameraobj->SetModel(model);

	srand(time(NULL));

	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].init(i, i);
		testEnemys[i].set({
			(float)(rand() % 50 - 25),
			(float)(rand() % 30),
			(float)(rand() % 50 - 25)
			});
	}

	testBoss.init(0, 0);
	testBoss.HP = 30;
}

//�f�o�b�O�e�L�X�g
void GameScene::debugs_print()
{
	debugtext.Print("W : Move Front", 10, 10, 1.0f);
	debugtext.Print("S : Move Back", 10, 25, 1.0f);
	debugtext.Print("MouseDrag : Camera(Left&Right)", 10, 55, 1.0f);
	debugtext.Print("MouseClick : Shot", 10, 70, 1.0f);
	debugtext.Print("MousePress(Left)&Drag : Target", 10, 85, 1.0f);
	debugtext.Print("MouseRelease : Homing", 10, 100, 1.0f);
	debugtext.Print("R : Reset", 10, 130, 1.0f);

	if (scene == title)
	{
		debugtext.Print("Title", 10, 160, 1.0f);
		debugtext.Print("SPACE : start", 10, 190, 1.0f);
	}
	else if (scene == play)
	{
		debugtext.Print("Play", 10, 160, 1.0f);
	}
	else
	{
		debugtext.Print("Result", 10, 160, 1.0f);
	}
}

#pragma region �e�V�[���X�V

//�^�C�g����ʍX�V
void GameScene::Title_update()
{
	if (input->Triger(DIK_SPACE))
	{
		up = 0;
		right = 0;
		objectRot = { 0,0,0 };

		pitch = 0.0f;
		yow = 0.0f;

		targetnum = 0;

		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].reSet();
		}

		testPlayer.reset();

		testBoss.reSet();
		testBoss.Isarive = false;
		testBoss.HP = 30;

		scene = play;
	}
}

//�v���C��ʍX�V
void GameScene::Play_update()
{
	//�v���C���[�X�V
	testPlayer.update();

	if (input->push(DIK_R))
	{
		up = 0;
		right = 0;
		objectRot = { 0,0,0 };

		pitch = 0.0f;
		yow = 0.0f;

		targetnum = 0;

		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].reSet();
		}

		testPlayer.reset();

		testBoss.reSet();
		testBoss.Isarive = false;
		testBoss.HP = 30;
	}

	skySphere->Update();

	//�G(�e�X�g)�X�V
	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].update(testPlayer.Player_object->getPosition());
		testPlayer.checkPlayerEnemy(&testEnemys[i]);
	}

	//�}�E�X�J�[�\����\��
	ShowCursor(false);

	//�G�����b�N�I������Ă��邩�ǂ���
	checkHitPlayerTarget();

	if (input->Mouse_LeftRelease())
	{
		for (int i = 0; i < targetnum; i++)
		{
			for (int j = 0; j < enemynum; j++)
			{
				if (testEnemys[j].Istarget_set && !testEnemys[j].IsSetMissile)
				{
					testPlayer.player_missiale[i].setPenemy(&testEnemys[j]);
					testPlayer.player_missiale[i].start(testPlayer.Player_object->getPosition());
					testEnemys[j].IsSetMissile = true;
					break;
				}
			}
		}

		targetnum = 0;
	}


	for (int j = 0; j < MaxPlayerBulletNum; j++)
	{
		for (int i = 0; i < enemynum; i++)
		{
			testPlayer.player_bullet[j].checkhit(&testEnemys[i]);
		}
	}

	int count = 0;

	for (int i = 0; i < enemynum; i++)
	{
		if (!testEnemys[i].Isarive)
		{
			count++;
		}
	}

	if (count >= enemynum || testPlayer.HP <= 0)
	{
		scene = clear;
	}
}

//���U���g��ʍX�V
void GameScene::Result_update()
{
	if (input->Triger(DIK_SPACE))
	{
		scene = title;
	}
}

#pragma endregion �e�V�[���X�V

#pragma region �e�V�[���`��

//�^�C�g����ʕ`��
void GameScene::Title_draw()
{

}

//�v���C��ʕ`��
void GameScene::Play_draw()
{
	skySphere->Draw(directx->cmdList.Get());

	//�v���C���[�`��
	testPlayer.draw_3d(directx);

	for (int i = 0; i < enemynum; i++)
	{
		//testEnemys[i].draw3D(directx);
	}
}

//���U���g��ʕ`��
void GameScene::Result_draw()
{
}

#pragma endregion �e�V�[���`��


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

	//camera->Update();

	debugs_print();
}

void GameScene::DrawBack()
{
	//�w�i�X�v���C�g�`��
	sample_back.DrawSprite(directx->cmdList.Get());
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
}

void GameScene::DrawSP()
{
	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].drawSp(directx);
	}

	if (scene == play)
	{
		testPlayer.draw_2d(directx);
	}
	debugtext.DrawAll(directx->cmdList.Get());
}

void GameScene::checkHitPlayerTarget()
{
	if (!testPlayer.Isrockon)
	{
		return;
	}

	if (!input->Mouse_LeftPush())
	{
		return;
	}

	for (int i = 0; i < enemynum; i++)
	{
		XMFLOAT2 screenPos = testEnemys[i].testObject->worldToScleen();

		float dis = sqrtf(powf(input->mouse_position.x - screenPos.x, 2) + powf(input->mouse_position.y - screenPos.y, 2));

		if (dis < 20 && !testEnemys[i].Istarget_set && targetnum < MaxPlayerMissileNum)
		{
			testEnemys[i].Istarget_set = true;
			targetnum++;
		}

		testEnemys[i].Rock_Target.position = { screenPos.x,screenPos.y,0 };
	}
}
