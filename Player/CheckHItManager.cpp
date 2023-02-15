#include"CheckHItManager.h"

// プレイヤー本体と敵本体の当たり判定(単体)
void checkHitManager::checkPlayerEnemy(Player* player, Enemy* enemy)
{
	if (enemy->Isarive == false || player->isArive == false)
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
	if (enemy->Isarive == false || bullet->isArive == false)
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

// プレイヤーの通常弾と敵本体の当たり判定(ボス)
void checkHitManager::checkBulletsEnemy(list<unique_ptr<bullet>>* bulletsList, Enemy* enemy)
{
	if (enemy->Isarive == false)
	{
		return;
	}

	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		checkBulletEnemy(newbullet.get(), enemy);
	}
}

// プレイヤーの通常弾と敵本体の当たり判定(群れ)
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

// プレイヤーの通常弾と敵の弾の当たり判定(単体)
void checkHitManager::checkBulletEnemybullet(bullet* bullet, Enemy* enemy)
{
	if (enemy->enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	if (enemy->bullet->isBulletArive() == false)
	{
		return;
	}

	if (bullet->isArive == false)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(bullet->bulletCollision, enemy->bullet->bulletCollision))
	{
		bullet->count = 0;
		bullet->isArive = false;

		enemy->bullet->isArive = false;
		enemy->bullet->ariveTime = 0;
	}
}

// プレイヤーの通常弾と敵の弾の当たり判定(ボス)
void checkHitManager::checkBulletsEnemybullet(list<unique_ptr<bullet>>* bulletsList, Enemy* enemy)
{
	for (std::unique_ptr<bullet>& newbullet : *bulletsList)
	{
		checkBulletEnemybullet(newbullet.get(), enemy);
	}
}

// プレイヤーの通常弾と敵の弾の当たり判定(群れ)
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

// プレイヤーのミサイルと敵の当たり判定
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
					newmissile->enemyPointer->HP = 0;
				}
				newmissile->isTargetSet = false;
				newmissile->enemyPointer->isTargetSet = false;
				newmissile->enemyPointer->isSetMissile = false;
				newmissile->enemyPointer = nullptr;

			}
		}
	}
}

// プレイヤー本体と敵の弾の当たり判定(単体)
void checkHitManager::chackPlayerEnemyBullet(Player* player, Enemy* enemy)
{
	if (enemy->enemyMovePattern != enemyPattern::shot)
	{
		return;
	}

	if (enemy->bullet->isBulletArive() == false)
	{
		return;
	}

	if (player->isArmor)
	{
		return;
	}

	if (Collision::CheckSphere2Sphere(player->playerCollision, enemy->bullet->getCollision()))
	{
		player->playerHP--;
		player->isArmor = true;
		enemy->bullet->isArive = false;
		enemy->bullet->ariveTime = 0;
	}
}

// プレイヤー本体と敵の弾の当たり判定(群れ)
void checkHitManager::checkPlayerEnemyBullets(Player* player, list<unique_ptr<Enemy>>* enemys)
{
	for (std::unique_ptr<Enemy>& newenemy : *enemys)
	{
		chackPlayerEnemyBullet(player, newenemy.get());
	}
}

// プレイヤーの通常弾と敵の乱射弾の当たり判定(単体同士)
void checkHitManager::checkBulletEnemyRampage(bullet* playerbullet, enemyBullet* rampagebullet)
{
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

// プレイヤーの通常弾と敵の乱射弾の当たり判定(list同士)
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

// プレイヤー本体と敵の乱射弾の当たり判定(単体)
void checkHitManager::checkPlayerEnemyRampage(Player* player, enemyBullet* rampagebullet)
{
	if (rampagebullet->isBulletArive() == false)
	{
		return;
	}

	if (player->isArmor)
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

// プレイヤー本体と敵の乱射弾の当たり判定(リスト)
void checkHitManager::checkPlayerEnemyRampages(Player* player, Enemy* enemy)
{
	for (std::unique_ptr<enemyBullet>& bullet : enemy->Bullets)
	{
		checkPlayerEnemyRampage(player, bullet.get());
	}
}

// プレイヤーカーソルと敵本体のロックオン判定(単体)
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

	//ロックオンした状態で離れすぎると解除
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

		targetnum++;
	}
}

// プレイヤーカーソルと敵本体のロックオン判定(群れ)
void checkHitManager::checkRockonEnemys(Player* player, list<unique_ptr<Enemy>>* enemys, int& targetnum)
{
	for (std::unique_ptr<Enemy>& newenemy : *enemys)
	{
		checkRockonEnemy(player, newenemy.get(), targetnum);
	}
}