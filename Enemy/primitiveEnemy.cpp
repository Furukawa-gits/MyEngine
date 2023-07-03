#include "primitiveEnemy.h"

directX* primitiveEnemy::directx = nullptr;
std::unique_ptr<Model> primitiveEnemy::staticEnemyModel = std::make_unique<Model>();
const XMFLOAT3 primitiveEnemy::miniMapPosition = { 0,0,0 };
const float primitiveEnemy::forPlayer = 400.0f;
XMFLOAT3 primitiveEnemy::playerPosition = { 0,0,0 };
XMFLOAT3 primitiveEnemy::playerFront = { 0,0,0 };
bool primitiveEnemy::playerIsAlive = false;
const XMFLOAT2 primitiveEnemy::defaultRockIconSize = { 0,0 };
const XMFLOAT2 primitiveEnemy::setRockIconSize = { 0,0 };
const float primitiveEnemy::decreaseSize = 0;

primitiveEnemy::primitiveEnemy()
{
}

primitiveEnemy::~primitiveEnemy()
{
}

void primitiveEnemy::staticDataInit()
{
	staticEnemyModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_02"));

	SingleParticle::loadTexInMap("bomb.png");
	SingleParticle::loadTexInMap("smoke.png");
	SingleParticle::loadTexInMap("enemy_chip.png");
}

void primitiveEnemy::staticDataUpdata(XMFLOAT3 playerpos, XMFLOAT3 playerfront, bool playerisarive)
{
	playerPosition = playerpos;
	playerFront = playerfront;
	playerIsAlive = playerisarive;
}
