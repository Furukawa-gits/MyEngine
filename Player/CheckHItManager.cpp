#include"CheckHItManager.h"

// �v���C���[�{�̂ƓG�{�̂̓����蔻��(�P��)
void checkHitManager::checkPlayerEnemy(Player* player, Enemy* enemy)
{
	if (!enemy->Isarive || !player->isArive)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(player->playerCollision, enemy->enemyCollision))
	{
		player->playerHP--;
		enemy->Isarive = false;
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
	if (!enemy->Isarive || !bullet->isArive)
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

// �v���C���[�̒ʏ�e�ƓG�̒e�̓����蔻��(�P��)
void checkHitManager::checkBulletEnemybullet(bullet* bullet, Enemy* enemy)
{
	if (enemy->enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	if (!enemy->bullet.isBulletArive())
	{
		return;
	}

	if (!bullet->isArive)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(bullet->bulletCollision, enemy->bullet.bulletCollision))
	{
		bullet->count = 0;
		bullet->isArive = false;

		enemy->bullet.isArive = false;
		enemy->bullet.ariveTime = 0;
	}
}

// �v���C���[�̒ʏ�e�ƓG�̒e�̓����蔻��(�{�X)
void checkHitManager::checkBulletsEnemybullet(list<unique_ptr<bullet>>* bulletsList, Enemy* enemy)
{
	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		checkBulletEnemybullet(newbullet.get(), enemy);
	}
}

// �v���C���[�̒ʏ�e�ƓG�̒e�̓����蔻��(�Q��)
void checkHitManager::checkBulletsEnemyBullets(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		for (std::unique_ptr<Enemy>& newenemy : *enemys)
		{
			checkBulletEnemybullet(newbullet.get(), newenemy.get());
		}
	}
}

// �v���C���[�̒ǔ��e�ƓG�̓����蔻��
void checkHitManager::checkMissilesEnemy(list<unique_ptr<Missile>>* missilesList)
{
	for (std::unique_ptr<Missile>& newmissile : *missilesList)
	{
		if (newmissile->enemyPointer == nullptr)
		{
			return;
		}

		if (newmissile->enemyPointer->Isarive == true && newmissile->isArive == true)
		{
			if (Collision::CheckSphere2Sphere(newmissile->missileCollision, newmissile->enemyPointer->enemyCollision))
			{
				if (newmissile->enemyPointer->isThisBoss)
				{
					newmissile->enemyPointer->HP--;

					if (newmissile->enemyPointer->HP <= 0)
					{
						newmissile->enemyPointer->Isarive = false;
					}
				}
				else
				{
					newmissile->enemyPointer->Isarive = false;
				}
				newmissile->isArive = false;
				newmissile->isTargetSet = false;
				newmissile->enemyPointer->isTargetSet = false;
				newmissile->enemyPointer->isSetMissile = false;
				newmissile->enemyPointer = nullptr;

			}
		}
	}
}

// �v���C���[�{�̂ƓG�̒e�̓����蔻��(�P��)
void checkHitManager::chackPlayerEnemyBullet(Player* player, Enemy* enemy)
{
	if (enemy->enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	if (!enemy->bullet.isBulletArive())
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(player->playerCollision, enemy->bullet.getCollision()))
	{
		player->playerHP--;
		enemy->bullet.isArive = false;
		enemy->bullet.ariveTime = 0;
	}
}

// �v���C���[�{�̂ƓG�̒e�̓����蔻��(�Q��)
void checkHitManager::checkPlayerEnemyBullets(Player* player, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<Enemy>& newenemy : *enemys)
	{
		chackPlayerEnemyBullet(player, newenemy.get());
	}
}

// �v���C���[�J�[�\���ƓG�{�̂̃��b�N�I������(�P��)
void checkHitManager::checkRockonEnemy(Player* player, Enemy* enemy, int& targetnum)
{
	XMFLOAT3 playerToEnemy =
	{
		enemy->position.x - player->playerObject->getPosition().x,
		enemy->position.y - player->playerObject->getPosition().y,
		enemy->position.z - player->playerObject->getPosition().z,
	};

	float length = sqrtf(powf(playerToEnemy.x, 2) + powf(playerToEnemy.y, 2) + powf(playerToEnemy.z, 2));

	if (length >= Enemy::forPlayer)
	{
		enemy->isTargetSet = false;
		enemy->isSetMissile = false;

		return;
	}

	XMFLOAT2 screenPos = enemy->enemyObject->worldToScleen();

	float dis = sqrtf(powf(player->targetFirst.position.x - screenPos.x, 2) + powf(player->targetFirst.position.y - screenPos.y, 2));

	if (dis < 56.5685f && !enemy->isTargetSet && targetnum < MaxPlayerMissileNum && enemy->isDraw)
	{
		enemy->isTargetSet = true;
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