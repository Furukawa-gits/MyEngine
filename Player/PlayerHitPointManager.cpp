#include "PlayerHitPointManager.h"

PlayerHitPointManager::PlayerHitPointManager()
{
}

PlayerHitPointManager::~PlayerHitPointManager()
{
}

void PlayerHitPointManager::loadSprites()
{
	//HP�Q�[�W
	HPGaugeBar.size = { 50,20 };
	HPGaugeBar.position = { 20,640,0 };
	HPGaugeBar.generateSprite("playerHPGauge.png");

	//HP�o�[�̌x��
	dangarHPGaugeBar.size = { (float)maxHP * 40,20 };
	dangarHPGaugeBar.position = { 20,640,0 };
	dangarHPGaugeBar.generateSprite("bossHPGauge.png");

	//�_���[�W�G�t�F�N�g
	damage.size = { 1280,720 };
	damage.generateSprite("damage.png");
}

void PlayerHitPointManager::Damage(int damage)
{
	if (!isAlive || isArmor)
	{
		return;
	}

	playerHP -= damage;

	if (playerHP <= 0)
	{
		isAlive = false;
	}

	isArmor = true;
	armorTime = 0;
}

void PlayerHitPointManager::reSet()
{
	isAlive = true;
	isDangerHP = false;
	isArmor = false;
	playerHP = maxHP;
	armorTime = 0;
	cautionHPCount = 0;
}

void PlayerHitPointManager::updata(int* isinvisible)
{
	if (!isAlive)
	{
		return;
	}

	//�U����A�������t���[���ŐH���Ȃ��悤�ɂ���
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

	//HP��4�����Ȃ�HP�o�[�_��
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

	if (playerHP <= 0)
	{
		isAlive = false;
	}

	//�_���[�W�\���X�v���C�g
	damage.spriteUpdata();

	//HP�Q�[�W�̍X�V
	HPGaugeBar.size = { (float)playerHP * 40,20 };
	HPGaugeBar.spriteUpdata();
	dangarHPGaugeBar.spriteUpdata();
}

void PlayerHitPointManager::drawHPUI(directX* directx)
{
	if (isArmor && armorTime < 5)
	{
		damage.drawSprite(directx->cmdList.Get());
	}

	if (isDangerHP)
	{
		dangarHPGaugeBar.drawSprite(directx->cmdList.Get());
	}
	HPGaugeBar.drawSprite(directx->cmdList.Get());
}
