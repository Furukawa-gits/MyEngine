#pragma once
#include "Boss.h"


class uniteBoss
	:public Boss
{
public:

	void initUniteBoss();

	void updateUnitBoss();

	void selectMovePattern();

	void drawUniteBoss();

private:

	std::unique_ptr<Boss> motherBoss;

	std::list<std::unique_ptr<Boss>> parts;
};