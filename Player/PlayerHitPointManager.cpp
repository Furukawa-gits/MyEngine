#include "PlayerHitPointManager.h"

PlayerHitPointManager::PlayerHitPointManager()
{
}

PlayerHitPointManager::~PlayerHitPointManager()
{
}

void PlayerHitPointManager::init()
{
	//リソース
	loadSprites();

	//パラメータ
	reSet();
}

void PlayerHitPointManager::loadSprites()
{
	//HPゲージ
	HPGaugeBar.size = { 50,20 };
	HPGaugeBar.position = { 20,640,0 };
	HPGaugeBar.generateSprite("playerHPGauge.png");

	//HPバーの警告
	dangarHPGaugeBar.size = { (float)maxHP * 40,20 };
	dangarHPGaugeBar.position = { 20,640,0 };
	dangarHPGaugeBar.generateSprite("bossHPGauge.png");

	//ダメージエフェクト
	damage.size = { 1280,720 };
	damage.generateSprite("damage.png");
}

void PlayerHitPointManager::Damage(int damage)
{
	if (!isAlive)
	{
		return;
	}

	playerHP -= damage;

	if (playerHP <= 0)
	{
		isAlive = false;
	}
}

void PlayerHitPointManager::reSet()
{
	isDangerHP = false;
	isArmor = false;
	playerHP = maxHP;
	armorTime = 0;
}

void PlayerHitPointManager::updata(int* isinvisible)
{
	//攻撃を連続したフレームで食らわないようにする
	if (isArmor)
	{
		armorTime++;

		if (armorTime % 10 == 0)
		{
			*isinvisible *= -1;
		}

		if (armorTime >= maxArmorTime)
		{
			armorTime = 0;
			isArmor = false;
			*isinvisible = -1;
		}
	}
	else
	{
		armorTime = 0;
	}

	//HPが4未満ならHPバー点滅
	if (playerHP < 4)
	{
		cautionHPCount++;

		if (cautionHPCount % 7 == 0)
		{
			isDangerHP = !isDangerHP;
		}
	}
	else
	{
		cautionHPCount = 0;
		isDangerHP = false;
	}

	//ダメージ表現スプライト
	damage.spriteUpdata();

	//HPゲージの更新
	HPGaugeBar.size = { (float)playerHP * 40,20 };
	HPGaugeBar.spriteUpdata();
}

void PlayerHitPointManager::drawHPUI(directX* directx)
{
	if (isDangerHP)
	{
		dangarHPGaugeBar.drawSprite(directx->cmdList.Get());
	}
	HPGaugeBar.drawSprite(directx->cmdList.Get());
}
