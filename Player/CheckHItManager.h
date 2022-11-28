#pragma once
#include<memory>
#include<list>
#include"Player.h"
#include"Enemy.h"
#include"Bullets.h"

using namespace std;

//やってること一覧
//プレイヤーと敵本体の当たり判定
//プレイヤーの通常弾と敵の当たり判定
//プレイヤーの追尾弾と敵の当たり判定
//プレイヤーと敵の弾の当たり判定
//プレイヤーと敵のロックオン判定

#pragma region プレイヤー本体と敵本体の当たり判定
/// <summary>
/// プレイヤーと敵本体の当たり判定(単体)
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="enemy">敵(単体)</param>
void checkPlayerEnemy(Player* player, Enemy* enemy);

/// <summary>
/// プレイヤーと敵本体の当たり判定(群れ)
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="enemys">敵(群れ)</param>
void checkPlayerEnemys(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion

#pragma region プレイヤーの通常弾と敵本体の当たり判定
/// <summary>
/// プレイヤーの通常弾と敵の当たり判定(単体)
/// </summary>
/// <param name="bulletsList">プレイヤーの通常弾</param>
/// <param name="enemy">敵(単体)</param>
void checkBulletEnemy(bullet* bullet, Enemy* enemy);

/// <summary>
/// プレイヤーの通常弾と敵の当たり判定(群れ)
/// </summary>
/// <param name="bulletsList">プレイヤーの通常弾</param>
/// <param name="enemys">敵(群れ)</param>
void checkBulletEnemys(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys);
#pragma endregion 

#pragma region プレイヤーの通常弾と敵の弾の当たり判定
/// <summary>
/// プレイヤーの通常弾と敵の当たり判定(単体)
/// </summary>
/// <param name="bulletsList">プレイヤーの通常弾</param>
/// <param name="enemy">敵(単体)</param>
void checkBulletEnemybullet(bullet* bullet, Enemy* enemy);

/// <summary>
/// プレイヤーの通常弾と敵の当たり判定(群れ)
/// </summary>
/// <param name="bulletsList">プレイヤーの通常弾</param>
/// <param name="enemys">敵(群れ)</param>
void checkBulletEnemyBullets(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys);
#pragma endregion 

#pragma region プレイヤーの追尾弾と敵本体の当たり判定
/// <summary>
/// プレイヤーの追尾弾と敵の当たり判定
/// </summary>
/// <param name="missilesList">プレイヤーの追尾弾</param>
void checkHomingEnemy(list<unique_ptr<Missile>>* missilesList);
#pragma endregion

#pragma region プレイヤー本体と敵の弾の当たり判定
/// <summary>
/// プレイヤーと敵の弾の当たり判定(単体)
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="enemy">敵(単体)</param>
void chackPlayerEnemyBullet(Player* player, Enemy* enemy);

/// <summary>
/// プレイヤーと敵の弾の当たり判定(群れ)
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="enemys">敵(群れ)</param>
void checkPlayerEnemyBullets(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion

#pragma region プレイヤーと敵のロックオン判定
/// <summary>
/// プレイヤーと敵のロックオン判定(単体)
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
void checkRockonEnemy(Player* player, Enemy* enemy);

/// <summary>
/// プレイヤーと敵のロックオン判定(群れ)
/// </summary>
/// <param name="player"></param>
/// <param name="enemys"></param>
void checkRockonEnemys(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion