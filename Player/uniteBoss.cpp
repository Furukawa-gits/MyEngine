#include "uniteBoss.h"

//static�ϐ�(�p�[�c)
float uniteParts::defaultPosition = 0.0f;
//static�ϐ�(�{��)
const float uniteBoss::partsRadius = 5.0f;
Player* uniteBoss::player = nullptr;

#pragma region �p�[�c
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
#pragma endregion �p�[�c

#pragma region �{��
uniteBoss::uniteBoss()
{
}

uniteBoss::~uniteBoss()
{
}

void uniteBoss::uniteBossInit()
{
	init(enemyPattern::shot);
	
	//�p�[�c���������e�I�u�W�F�N�g�ݒ�
	for (int i = 0; i < unitNum; i++)
	{
		
	}
}

void uniteBoss::uniteBossUpdata()
{
	//�{�̍X�V


	//�p�[�c�X�V

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
#pragma endregion �{��
