#include"CheckHItManager.h"

// プレイヤー本体と敵本体の当たり判定(単体)
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

// プレイヤー本体と敵本体の当たり判定(群れ)
void checkHitManager::checkPlayerEnemys(Player* player, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<Enemy>& newenemy : *enemys)
	{
		checkPlayerEnemy(player, newenemy.get());
	}
}

// プレイヤーの通常弾と敵本体の当たり判定(単体)
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

// プレイヤーの通常弾と敵本体の当たり判定(群れ)
void checkHitManager::checkBulletEnemys(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		for (std::unique_ptr<Enemy>& newenemy : *enemys)
		{
			checkBulletEnemy(newbullet.get(), newenemy.get());
		}
	}
}

// プレイヤーの通常弾と敵の弾の当たり判定(単体)
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

// プレイヤーの通常弾と敵の弾の当たり判定(群れ)
void checkHitManager::checkBulletEnemyBullets(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		for (std::unique_ptr<Enemy>& newenemy : *enemys)
		{
			checkBulletEnemybullet(newbullet.get(), newenemy.get());
		}
	}
}

// プレイヤーの追尾弾と敵の当たり判定
void checkHitManager::checkHomingEnemy(list<unique_ptr<Missile>>* missilesList)
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

// プレイヤーと敵の弾の当たり判定(単体)
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

// プレイヤーと敵の弾の当たり判定(群れ)
void checkHitManager::checkPlayerEnemyBullets(Player* player, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<Enemy>& newenemy : *enemys)
	{
		chackPlayerEnemyBullet(player, newenemy.get());
	}
}

// プレイヤーと敵のロックオン判定(単体)
void checkHitManager::checkRockonEnemy(Player* player, Enemy* enemy, int& targetnum)
{
	XMFLOAT2 screenPos = enemy->enemyObject->worldToScleen();

	float dis = sqrtf(powf(player->targetFirst.position.x - screenPos.x, 2) + powf(player->targetFirst.position.y - screenPos.y, 2));

	if (dis < 56.5685f && !enemy->isTargetSet && targetnum < MaxPlayerMissileNum && enemy->isDraw)
	{
		enemy->isTargetSet = true;
		targetnum++;
	}
}

// プレイヤーと敵のロックオン判定(群れ)
void checkHitManager::checkRockonEnemys(Player* player, list<unique_ptr<Enemy>>* enemys, int& targetnum)
{
	for (std::unique_ptr<Enemy>& newenemy : *enemys)
	{
		checkRockonEnemy(player, newenemy.get(), targetnum);
	}
}