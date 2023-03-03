#include "uniteBoss.h"

//static変数(パーツ)
float uniteParts::defaultPosition = 0.0f;
//static変数(本体)
const float uniteBoss::partsRadius = 5.0f;
Player* uniteBoss::player = nullptr;

#pragma region パーツ
void uniteParts::initParts(int ID)
{
	init(enemyPattern::shot);
}

void uniteParts::setStaticData(XMFLOAT3* motherposition)
{
}

void uniteParts::updataParts()
{
	
}

void uniteParts::setParts(XMFLOAT3 position, float theta, float phi)
{

}
#pragma endregion パーツ

#pragma region 本体
uniteBoss::uniteBoss()
{
}

uniteBoss::~uniteBoss()
{
}

void uniteBoss::uniteBossInit()
{
	init(enemyPattern::shot);
	
	//パーツ初期化＆親オブジェクト設定
	for (int i = 0; i < unitNum; i++)
	{
		
	}
}

void uniteBoss::uniteBossUpdata()
{
	//本体更新


	//パーツ更新

}

void uniteBoss::uniteBossArrival(Player* player)
{
	if (!isAppear)
	{
		return;
	}

	isAppear = true;
}

void uniteBoss::uniteBossDeathMove()
{
}

void uniteBoss::uniteBossSet()
{
	HP = 11;
}

void uniteBoss::selectMovePattern()
{
	int test = rand() % sizeof(enemyPattern);
}

void uniteBoss::uniteBossDraw3d(directX* directx)
{
	draw3D(directx);
}

void uniteBoss::uniteBossDraw2d(directX* directx)
{
	draw2D(directx);
}
#pragma endregion 本体
