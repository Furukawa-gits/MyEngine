#include "primitiveScene.h"

directX* primitiveScene::directx = nullptr;
dxinput* primitiveScene::input = nullptr;
Audio* primitiveScene::audio = nullptr;
Light* primitiveScene::light = nullptr;
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
float primitiveScene::nextWaveDis = 0.0f;

void primitiveScene::setStaticData(directX* Directx, dxinput* Input, Audio* Audio)
{
	directx = Directx;
	input = Input;
	audio = Audio;

	//スプライトクラス初期化
	SingleSprite::setStaticData(directx->dev.Get(), input);

	//パーティクル初期化
	SingleParticle::particleStaticInit(directx, nullptr);

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

	//敵モデルの初期化
	Enemy::staticInit();

	//ボスの初期化
	Boss::staticInitBoss();
	normalBoss->bossInit();

	//ボス(ユニット)の初期化
	uniteBoss::uniteBossStaticInit(playerPointer.get());
	UniteBoss->uniteBossInit();
}
