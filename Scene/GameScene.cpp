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

	target.anchorpoint = { 0.5f,0.5f };
	//target.size = { 50.0f,50.0f };
	target.GenerateSprite("Target.png");
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
	Object3d_FBX::SetDevice(directx->dev.Get());

	Object3d_FBX::SetCamera(camera);

	Object3d_FBX::CreateGraphicsPipeline();

	Object3d_FBX::CreateGraphicsPipelineSimple();

	model = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");
	SkyModel = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");

	object = new Object3d_FBX;
	object->Initialize();
	object->SetModel(model);
	object->SetPosition({ 0,5,0 });
	object->setSpeed(2.0f);
	object->PlayAnimation();

	followcamera = new FollowCamera();

	followcamera->setFollowTarget(&object->getPosition(), &object->getRotation(), -30);

	Object3d_FBX::SetCamera(followcamera);

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
			(float)(rand() % 30 - 15),
			(float)(rand() % 20),
			(float)(rand() % 30 - 15)
			});
	}
}

//�f�o�b�O�e�L�X�g
void GameScene::debugs_print()
{
	debugtext.Print("W : Move Front", 10, 10, 1.0f);
	//debugtext.Print("S : Move Back", 10, 25, 1.0f);
	debugtext.Print("MouseDrag : Camera(Left&Right)", 10, 55, 1.0f);
	debugtext.Print("MouseClick : Shot", 10, 70, 1.0f);
	debugtext.Print("MousePress(Left)&Drag : Target", 10, 85, 1.0f);
	debugtext.Print("MouseRelease : Homing", 10, 100, 1.0f);
	debugtext.Print("R : Reset", 10, 130, 1.0f);

	debugtext.Print("1 : Object Simple", 1000, 10, 1.0f);
	debugtext.Print("2 : posteffect GrayScale", 1000, 30, 1.0f);
}

#pragma region �e�V�[���X�V

//�^�C�g����ʍX�V
void GameScene::Title_update()
{
	if (input->mouse_p.x >= 1000)
	{
		objectRot.y += 0.7f;
		yow += 0.7f;
	}

	if (input->mouse_p.x <= 280)
	{
		objectRot.y -= 0.7f;
		yow -= 0.7f;
	}

	//�O�ɐi��
	if (input->push(DIK_W))
	{
		object->addMoveFront(followcamera->getFrontVec());
	}
	//��ɉ�����
	if (input->push(DIK_S))
	{
		object->addMoveBack(followcamera->getFrontVec());
	}

	if (input->push(DIK_R))
	{
		up = 0;
		right = 0;
		objectRot = { 0,0,0 };
		object->SetPosition({ 0,5,0 });

		pitch = 0.0f;
		yow = 0.0f;

		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].reSet();
		}
	}

	object->SetRotation({ pitch,yow,roll });
	XMVECTOR matQ = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yow), XMConvertToRadians(roll));
	object->addQRot(matQ);

	followcamera->TargetObjectPos = &object->getPosition();
	followcamera->TargetObjectAngle = &object->getRotation();

	followcamera->Following();

	followcamera->setFrontVec(0.5f);

	object->Update();
	skySphere->Update();

	cameraobj->SetPosition(followcamera->GetEye());
	cameraobj->SetRotation(*followcamera->TargetObjectAngle);
	cameraobj->Update();

	//�G(�e�X�g)������
	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].update(object->getPosition());
	}

	//�}�E�X�J�[�\����\��
	ShowCursor(false);

	if (input->Mouse_LeftPush())
	{
		mousePressCount++;
	}
	else
	{
		mousePressCount = 0;
	}

	if (mousePressCount > 30)
	{
		target.rotation -= 5.0f;
		isTarget = true;
	}
	else
	{
		target.rotation += 3.0f;
		isTarget = false;
	}

	//�G�����b�N�I������Ă��邩�ǂ���
	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].isHitTarget({ target.position.x,target.position.y }, isTarget);
	}

	target.position = MOUSE_POS;
	target.SpriteTransferVertexBuffer();
	target.SpriteUpdate();

	if (input->Mouse_LeftRelease())
	{
		for (int i = 0; i < enemynum; i++)
		{
			if (testEnemys[i].Istarget_set)
			{
				testEnemys[i].HP--;
			}
		}
	}

	if (input->Mouse_LeftTriger())
	{
		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].isHitShot({ target.position.x,target.position.y });
		}
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

#pragma endregion �e�V�[���X�V

#pragma region �e�V�[���`��

//�^�C�g����ʕ`��
void GameScene::Title_draw()
{
	skySphere->Draw(directx->cmdList.Get());

	//directx->depthclear();
	object->Draw(directx->cmdList.Get());

	if (isEnemySimple)
	{
		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].testObject->SetPipelineSimple(directx->cmdList.Get());
		}
	}
	else
	{
		for (int i = 0; i < enemynum; i++)
		{
			testEnemys[i].testObject->reSetPipeline();
		}
	}

	for (int i = 0; i < enemynum; i++)
	{
		testEnemys[i].draw3D(directx);
	}

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

	camera->Update();

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

	target.DrawSprite(directx->cmdList.Get());

	debugtext.DrawAll(directx->cmdList.Get());
}
