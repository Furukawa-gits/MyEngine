#include "primitiveScene.h"

directX* primitiveScene::directx = nullptr;
dxinput* primitiveScene::input = nullptr;
Audio* primitiveScene::audio = nullptr;
Light* primitiveScene::light = nullptr;
std::unique_ptr<SingleSprite> primitiveScene::sample_back = std::make_unique<SingleSprite>();
std::unique_ptr<Model> primitiveScene::SkyModel = std::make_unique<Model>();
std::unique_ptr<Model> primitiveScene::groundModel = std::make_unique<Model>();
std::unique_ptr<object3dFBX> primitiveScene::skySphere = std::make_unique<object3dFBX>();
std::unique_ptr<object3dFBX> primitiveScene::groundPlane = std::make_unique<object3dFBX>();
std::list<std::unique_ptr<Enemy>> primitiveScene::enemyList = {};
std::unique_ptr<Boss> primitiveScene::normalBoss = std::make_unique<Boss>();
std::unique_ptr<uniteBoss> primitiveScene::UniteBoss = std::make_unique<uniteBoss>();
std::unique_ptr<Player> primitiveScene::playerPointer = std::make_unique<Player>();
std::unique_ptr<SingleSprite> primitiveScene::enemyWaveBar = std::make_unique<SingleSprite>();
std::unique_ptr<SingleSprite> primitiveScene::playerWaveIcon = std::make_unique<SingleSprite>();
std::list<std::unique_ptr<SingleSprite>> primitiveScene::enemyWaveIcons = {};
int primitiveScene::stageNum = 1;
int primitiveScene::nowStageLevel = 1;
int primitiveScene::maxStageLevel = 1;
bool primitiveScene::isTutorial = false;
bool primitiveScene::isClearOrOver = false;
bool primitiveScene::isSelectOrTitle = false;
float primitiveScene::nextWaveDis = 0.0f;

primitiveScene::primitiveScene()
{

}

primitiveScene::~primitiveScene()
{

}

void primitiveScene::setStaticData(directX* Directx, dxinput* Input, Audio* Audio)
{
	directx = Directx;
	input = Input;
	audio = Audio;

	//�X�v���C�g�N���X������
	SingleSprite::setStaticData(directx->dev.Get(), input);

	//�p�[�e�B�N��������
	SingleParticle::particleStaticInit(directx, nullptr);

	//�w�i
	sample_back->size = { 1280,720 };
	sample_back->generateSprite("sample_back.jpg");

	//���C�g����
	Light::Staticinitialize(directx->dev.Get());
	light = Light::Create();
	light->SetLightColor({ 1,1,1 });
	light->SetLightDir({ 0,-1,0,0 });

	//3d�I�u�W�F�N�g����
	object3dFBX::setLight(light);
	object3dFBX::SetDevice(directx->dev.Get());
	object3dFBX::CreateGraphicsPipeline();
	object3dFBX::CreateGraphicsPipelineSimple();

	//�v���C���[������
	playerPointer->init(input, directx);

	Model* sky = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");
	Model* ground = FbxLoader::GetInstance()->LoadmodelFromFile("floar");

	SkyModel = std::make_unique<Model>();
	SkyModel.reset(sky);
	groundModel = std::make_unique<Model>();
	groundModel.reset(ground);

	skySphere = std::make_unique<object3dFBX>();
	skySphere->initialize();
	skySphere->SetModel(SkyModel.get());
	skySphere->SetScale({ 8.0f,8.0f,8.0f });

	groundPlane = std::make_unique<object3dFBX>();
	groundPlane->initialize();
	groundPlane->SetModel(groundModel.get());
	groundPlane->SetPosition(playerPointer->groundPosition);
	groundPlane->SetScale({ 0.5f,0.5f,0.5f });

	//�p�[�e�B�N���̋��ʃJ������ݒ�
	SingleParticle::setCamera(playerPointer->followCamera);

	//�G���f���̏�����
	Enemy::staticInit();

	//�{�X�̏�����
	Boss::staticInitBoss();
	normalBoss->bossInit();

	//�{�X(���j�b�g)�̏�����
	uniteBoss::uniteBossStaticInit(playerPointer.get());
	UniteBoss->uniteBossInit();

	//�}�E�X�J�[�\����\��
	ShowCursor(false);
}

void primitiveScene::finalize()
{
	enemyList.clear();

	Enemy::staticDestroy();
	enemyBullet::staticDestroy();
}
