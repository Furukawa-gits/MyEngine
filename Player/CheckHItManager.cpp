#include"CheckHItManager.h"

// �v���C���[�{�̂ƓG�{�̂̓����蔻��(�P��)
void checkHitManager::checkPlayerEnemy(Player* player, Enemy* enemy)
{
	if (enemy->isArive == false || player->isArive == false)
	{
		return;
	}

	if (player->isArmor)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(player->playerCollision, enemy->enemyCollision))
	{
		player->playerHP--;
		player->isArmor = true;

		if (!enemy->isThisBoss)
		{
			enemy->HP--;
		}
	}
}

// �v���C���[�{�̂ƓG�{�̂̓����蔻��(�Q��)
void checkHitManager::checkPlayerEnemys(Player* player, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<Enemy>& newenemy : *enemys)
	{
		checkPlayerEnemy(player, newenemy.get());
	}
}

// �v���C���[�̒ʏ�e�ƓG�{�̂̓����蔻��(�P��)
void checkHitManager::checkBulletEnemy(bullet* bullet, Enemy* enemy)
{
	if (enemy->isArive == false || bullet->isArive == false)
	{
		return;
	}

	if (enemy->isArmor)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(bullet->bulletCollision, enemy->enemyCollision))
	{
		bullet->count = 0;
		bullet->isArive = false;

		enemy->HP--;
	}
}

// �v���C���[�̒ʏ�e�ƓG�{�̂̓����蔻��(�{�X)
void checkHitManager::checkBulletsEnemy(list<unique_ptr<bullet>>* bulletsList, Enemy* enemy)
{
	if (enemy->isArive == false)
	{
		return;
	}

	if (enemy->isArmor)
	{
		return;
	}

	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		checkBulletEnemy(newbullet.get(), enemy);
	}
}

// �v���C���[�̒ʏ�e�ƓG�{�̂̓����蔻��(�Q��)
void checkHitManager::checkBulletsEnemys(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		for (std::unique_ptr<Enemy>& newenemy : *enemys)
		{
			checkBulletEnemy(newbullet.get(), newenemy.get());
		}
	}
}

// �v���C���[�̃~�T�C���ƓG�{�̂̓����蔻��(�P��)
void checkHitManager::checkMissilesEnemy(list<unique_ptr<Missile>>* missilesList)
{
	for (std::unique_ptr<Missile>& newmissile : *missilesList)
	{
		if (newmissile->enemyPointer == nullptr)
		{
			return;
		}

		if (newmissile->enemyPointer->isArive == true && newmissile->isArive == true)
		{
			if (Collision::CheckSphere2Sphere(newmissile->missileCollision, newmissile->enemyPointer->enemyCollision))
			{
				if (newmissile->enemyPointer->isThisBoss)
				{
					if (!newmissile->enemyPointer->isArmor)
					{
						newmissile->enemyPointer->HP--;
					}
				}
				else
				{
					if (!newmissile->enemyPointer->isArmor)
					{
						newmissile->enemyPointer->HP = 0;
					}
				}
				newmissile->isTargetSet = false;
				newmissile->enemyPointer->isTargetSet = false;
				newmissile->enemyPointer->isSetMissile = false;
				newmissile->enemyPointer = nullptr;

			}
		}
	}
}

// �v���C���[�̒ʏ�e�ƓG�̗��˒e�̓����蔻��(�P�̓��m)
void checkHitManager::checkBulletEnemyRampage(bullet* playerbullet, enemyBullet* rampagebullet)
{
	if (rampagebullet == nullptr)
	{
		return;
	}

	if (rampagebullet->isBulletArive() == false)
	{
		return;
	}

	if (playerbullet->isArive == false)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(playerbullet->bulletCollision, rampagebullet->bulletCollision))
	{
		playerbullet->count = 0;
		playerbullet->isArive = false;

		rampagebullet->isArive = false;
		rampagebullet->ariveTime = 0;
	}
}

// �v���C���[�̒ʏ�e�ƓG�̗��˒e�̓����蔻��(list���m)
void checkHitManager::checkBulletsEnemyRampage(list<unique_ptr<bullet>>* bulletsList, Enemy* enemy)
{
	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		for (std::unique_ptr<enemyBullet>& enemybullet : enemy->Bullets)
		{
			checkBulletEnemyRampage(newbullet.get(), enemybullet.get());
		}
	}
}

// �v���C���[�{�̂ƓG�̗��˒e�̓����蔻��(�P��)
void checkHitManager::checkPlayerEnemyRampage(Player* player, enemyBullet* rampagebullet)
{
	if (rampagebullet == nullptr)
	{
		return;
	}

	if (rampagebullet->isBulletArive() == false)
	{
		return;
	}

	if (player->isArmor || player->isArive == false)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(player->playerCollision, rampagebullet->getCollision()))
	{
		player->playerHP--;
		player->isArmor = true;
		rampagebullet->isArive = false;
		rampagebullet->ariveTime = 0;
	}
}

// �v���C���[�{�̂ƓG�̗��˒e�̓����蔻��(���X�g)
void checkHitManager::checkPlayerEnemyRampages(Player* player, Enemy* enemy)
{
	for (std::unique_ptr<enemyBullet>& bullet : enemy->Bullets)
	{
		checkPlayerEnemyRampage(player, bullet.get());
	}
}

// �v���C���[�J�[�\���ƓG�{�̂̃��b�N�I������(�P��)
void checkHitManager::checkRockonEnemy(Player* player, Enemy* enemy, int& targetnum)
{
	if (player->isRockOn == false)
	{
		return;
	}

	if (enemy->isDraw == false)
	{
		return;
	}

	if (enemy->isArmor)
	{
		return;
	}

	if (player->isArive == false)
	{
		return;
	}

	if (enemy->isArmor)
	{
		return;
	}

	//���b�N�I��������Ԃŗ��ꂷ����Ɖ���
	if (enemy->isFar == true && enemy->isTargetSet)
	{
		enemy->isTargetSet = false;

		if (targetnum > 0)
		{
			targetnum--;
		}
		return;
	}

	if (enemy->toPlayerAngle > 85)
	{
		return;
	}

	XMFLOAT2 screenPos = enemy->enemyObject->worldToScleen();

	float dis = sqrtf(powf(player->targetFirst.position.x - screenPos.x, 2) + powf(player->targetFirst.position.y - screenPos.y, 2));

	if (dis < 56.5685f && enemy->isTargetSet == false && targetnum < MaxPlayerMissileNum)
	{
		enemy->isTargetSet = true;
		enemy->rockTarget->size = Enemy::setRockIconSize;

		targetnum++;
	}
}

// �v���C���[�J�[�\���ƓG�{�̂̃��b�N�I������(�Q��)
void checkHitManager::checkRockonEnemys(Player* player, list<unique_ptr<Enemy>>* enemys, int& targetnum)
{
	for (std::unique_ptr<Enemy>& newenemy : *enemys)
	{
		checkRockonEnemy(player, newenemy.get(), targetnum);
	}
}