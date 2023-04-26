#include "primitiveScene.h"

directX* primitiveScene::directx = nullptr;
dxinput* primitiveScene::input = nullptr;
Audio* primitiveScene::audio = nullptr;
std::list<std::unique_ptr<Enemy>> primitiveScene::enemyList = {};
std::unique_ptr<Boss> primitiveScene::normalBoss = std::make_unique<Boss>();
std::unique_ptr<uniteBoss> primitiveScene::UniteBoss = std::make_unique<uniteBoss>();
std::unique_ptr<Player> primitiveScene::playerPointer = std::make_unique<Player>();
std::unique_ptr<SingleSprite> primitiveScene::enemyWaveBar = std::make_unique<SingleSprite>();
std::unique_ptr<SingleSprite> primitiveScene::playerWaveIcon = std::make_unique<SingleSprite>();
int primitiveScene::stageNum = 1;
int primitiveScene::nowStageLevel = 1;
int primitiveScene::maxStageLevel = 1;
bool primitiveScene::isTutorial = false;

void primitiveScene::setStaticData(directX* Directx, dxinput* Input, Audio* Audio)
{
	directx = Directx;
	input = Input;
	audio = Audio;
}
