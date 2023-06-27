#include "primitiveScene.h"

directX* primitiveScene::directx = nullptr;
dxinput* primitiveScene::input = nullptr;
Audio* primitiveScene::audio = nullptr;
Light* primitiveScene::light = nullptr;
std::unique_ptr<SingleSprite> primitiveScene::sample_back = std::make_unique<SingleSprite>();
std::unique_ptr<Model> primitiveScene::SkyModel = std::make_unique<Model>();
std::unique_ptr<Model> primitiveScene::SkyModel2 = std::make_unique<Model>();
std::unique_ptr<Model> primitiveScene::SkyModel3 = std::make_unique<Model>();
std::unique_ptr<Model> primitiveScene::groundModel = std::make_unique<Model>();
std::unique_ptr<object3dFBX> primitiveScene::skySphere = std::make_unique<object3dFBX>();
std::unique_ptr<object3dFBX> primitiveScene::skySphere2 = std::make_unique<object3dFBX>();
std::unique_ptr<object3dFBX> primitiveScene::skySphere3 = std::make_unique<object3dFBX>();
std::unique_ptr<object3dFBX> primitiveScene::groundPlane = std::make_unique<object3dFBX>();
std::list<std::unique_ptr<Enemy>> primitiveScene::enemyList = {};
std::unique_ptr<Boss> primitiveScene::normalBoss = std::make_unique<Boss>();
std::unique_ptr<uniteBoss> primitiveScene::UniteBoss = std::make_unique<uniteBoss>();
std::unique_ptr<Player> primitiveScene::playerPointer = std::make_unique<Player>();
std::unique_ptr<SingleSprite> primitiveScene::enemyWaveBar = std::make_unique<SingleSprite>();
std::unique_ptr<SingleSprite> primitiveScene::playerWaveIcon = std::make_unique<SingleSprite>();
std::list<std::unique_ptr<SingleSprite>> primitiveScene::enemyWaveIcons = {};
int primitiveScene::stageNum = 0;
XMFLOAT3 primitiveScene::stageIconOffset = { 340,360,0 };
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

	//スプライトクラス初期化
	SingleSprite::setStaticData(directx->dev.Get(), input);

	//パーティクル初期化
	SingleParticle::particleStaticInit(directx, nullptr);

	//背景
	sample_back->size = { 1280,720 };
	sample_back->generateSprite("sample_back.jpg");

	enemyWaveBar->anchorpoint = { 0.5f,0.0f };
	enemyWaveBar->size = { 10,500 };
	enemyWaveBar->position = { 1180,360 - 500 / 2,0 };
	enemyWaveBar->generateSprite("enemyWaveBar.png");

	playerWaveIcon->anchorpoint = { 0.5f,0.5f };
	playerWaveIcon->size = { 120,75 };
	playerWaveIcon->position = { 1180 + 20,360 - 500 / 2,0 };
	playerWaveIcon->generateSprite("playerWaveIcon.png");

	//ライト生成
	Light::Staticinitialize(directx->dev.Get());
	light = Light::Create();
	light->SetLightColor({ 1,1,1 });
	light->SetLightDir({ 0,-1,0,0 });

	//3dオブジェクト生成
	object3dFBX::setLight(light);
	object3dFBX::SetDevice(directx->dev.Get());
	object3dFBX::CreateGraphicsPipeline();
	object3dFBX::CreateGraphicsPipelineSimple();

	//プレイヤー初期化
	playerPointer->init(input, directx);

	Model* sky = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");
	Model* sky2 = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere2");
	Model* sky3 = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere3");
	Model* ground = FbxLoader::GetInstance()->LoadmodelFromFile("floar");

	SkyModel.reset(sky);
	SkyModel2.reset(sky2);
	SkyModel3.reset(sky3);
	groundModel.reset(ground);

	skySphere->initialize();
	skySphere->SetModel(SkyModel.get());
	skySphere->SetScale({ 8.0f,8.0f,8.0f });

	skySphere2->initialize();
	skySphere2->SetModel(SkyModel2.get());
	skySphere2->SetScale({ 0.4f,0.4f,0.4f });

	skySphere3->initialize();
	skySphere3->SetModel(SkyModel3.get());
	skySphere3->SetScale({ 0.4f,0.4f,0.4f });

	groundPlane->initialize();
	groundPlane->SetModel(groundModel.get());
	groundPlane->SetPosition(playerPointer->groundPosition);
	groundPlane->SetScale({ 0.5f,0.5f,0.5f });

	//パーティクルの共通カメラを設定
	SingleParticle::setCamera(playerPointer->followCamera);

	//敵モデルの初期化
	Enemy::staticInit();

	//ボスの初期化
	Boss::staticInitBoss();
	normalBoss->bossInit();

	//ボス(ユニット)の初期化
	uniteBoss::uniteBossStaticInit(playerPointer.get());
	UniteBoss->uniteBossInit();

	//マウスカーソル非表示
	ShowCursor(false);
}

void primitiveScene::finalize()
{
	enemyList.clear();

	Enemy::staticDestroy();
}
