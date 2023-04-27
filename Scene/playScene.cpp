#include "playScene.h"

playScene::playScene()
{
}

void playScene::loadResources()
{
#pragma region	//�`���[�g���A���p�̃e�L�X�g
	moveText.anchorpoint = { 0.5f,0.5f };
	moveText.size = { 375,75 };
	moveText.position = { 640,600,0 };
	moveText.generateSprite("moveText.png");

	boostText.anchorpoint = { 0.5f,0.5f };
	boostText.size = { 398,75 };
	boostText.position = { 640,600,0 };
	boostText.generateSprite("boostText.png");

	shotText.anchorpoint = { 0.5f,0.5f };
	shotText.size = { 300,75 };
	shotText.position = { 640,600,0 };
	shotText.generateSprite("shotText.png");

	missileText.anchorpoint = { 0.5f,0.5f };
	missileText.size = { 593,94 };
	missileText.position = { 640,600,0 };
	missileText.generateSprite("missileText.png");

	shootingText.anchorpoint = { 0.5f,0.5f };
	shootingText.size = { 398,75 };
	shootingText.position = { 640,600,0 };
	shootingText.generateSprite("shootingText.png");
#pragma endregion //�`���[�g���A���p�̃e�L�X�g

#pragma region	//�J�E���g�_�E���A�C�R��
	countDownSprite[0].anchorpoint = { 0.5f,0.5f };
	countDownSprite[0].size = { 100,100 };
	countDownSprite[0].position = { 640,360,0 };
	countDownSprite[0].generateSprite("count3.png");
	countDownSprite[0].spriteUpdata();

	countDownSprite[1].anchorpoint = { 0.5f,0.5f };
	countDownSprite[1].size = { 100,100 };
	countDownSprite[1].position = { 640,360,0 };
	countDownSprite[1].generateSprite("count2.png");
	countDownSprite[1].spriteUpdata();

	countDownSprite[2].anchorpoint = { 0.5f,0.5f };
	countDownSprite[2].size = { 100,100 };
	countDownSprite[2].position = { 640,360,0 };
	countDownSprite[2].generateSprite("count1.png");
	countDownSprite[2].spriteUpdata();
#pragma endregion //�J�E���g�_�E���A�C�R��

	//�X�^�[�g�e�L�X�g
	playStart.anchorpoint = { 0.5f,0.5f };
	playStart.size = { 500,125 };
	playStart.position = { 640,360,0 };
	playStart.generateSprite("playstart.png");

	//���x���[�^�[
	heightGauge.anchorpoint = { 0.5f,0.5f };
	heightGauge.size = { 40,200 };
	heightGauge.position = { Enemy::miniMapPosition.x + 120,Enemy::miniMapPosition.y,0 };
	heightGauge.generateSprite("playerheightbar.png");

	//�v���C���[�̍��x
	playerHeight.anchorpoint = { 0.5f,0.5f };
	playerHeight.size = { 32,5 };
	playerHeight.generateSprite("playerHPGauge.png");
	playerHeightIcon.anchorpoint = { 0.0f,0.5f };
	playerHeightIcon.size = { 44,17 };
	playerHeightIcon.generateSprite("playerHeightIcon.png");

	//�V���ƒn�ʂ̃��f��
	Model* sky = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");
	Model* ground = FbxLoader::GetInstance()->LoadmodelFromFile("floar");
	SkyModel = std::make_unique<Model>();
	SkyModel.reset(sky);
	groundModel = std::make_unique<Model>();
	groundModel.reset(ground);

	//�V���ƒn�ʂ̃I�u�W�F�N�g������
	skySphere = std::make_unique<object3dFBX>();
	skySphere->initialize();
	skySphere->SetModel(SkyModel.get());
	skySphere->SetScale({ 8.0f,8.0f,8.0f });
	groundPlane = std::make_unique<object3dFBX>();
	groundPlane->initialize();
	groundPlane->SetModel(groundModel.get());
	groundPlane->SetPosition(playerPointer->groundPosition);
	groundPlane->SetScale({ 0.5f,0.5f,0.5f });
}

void playScene::initialize()
{
}

void playScene::setParameter()
{

}

void playScene::updata()
{
}

void playScene::draw3D()
{
	groundPlane->Draw(directx->cmdList.Get());
	skySphere->Draw(directx->cmdList.Get());

	//�v���C���[�`��
	playerPointer->draw3D(directx);

	//�G
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw3D(directx);
	}

	//�{�X�`��
	normalBoss->bossDraw3D(directx);

	UniteBoss->uniteBossDraw3d(directx);
}

void playScene::draw2D()
{
}

void playScene::drawNowWave()
{
}

void playScene::drawPositionUI()
{
}

void playScene::checkHitPlayerTarget()
{
}

void playScene::checkHitEverything()
{
}

void playScene::countDown()
{
}
