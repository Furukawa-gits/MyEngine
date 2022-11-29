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

class checkHitManager
{
public:

#pragma region プレイヤー本体と敵本体の当たり判定
	/// <summary>
	/// プレイヤー本体と敵本体の当たり判定(単体)
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="enemy">敵(単体)</param>
	static void checkPlayerEnemy(Player* player, Enemy* enemy);

	/// <summary>
	/// プレイヤー本体と敵本体の当たり判定(群れ)
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="enemys">敵(群れ)</param>
	static void checkPlayerEnemys(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion

#pragma region プレイヤーの通常弾と敵本体の当たり判定
	/// <summary>
	/// プレイヤーの通常弾と敵本体の当たり判定(単体)
	/// </summary>
	/// <param name="bullet">プレイヤーの通常弾</param>
	/// <param name="enemy">敵(単体)</param>
	static void checkBulletEnemy(bullet* bullet, Enemy* enemy);

	/// <summary>
	/// プレイヤーの通常弾と敵の当たり判定(ボス)
	/// </summary>
	/// <param name="bulletsList">プレイヤーの通常弾</param>
	/// <param name="enemy">敵(単体)</param>
	static void checkBulletsEnemy(list<unique_ptr<bullet>>* bulletsList, Enemy* enemy);

	/// <summary>
	/// プレイヤーの通常弾と敵本体の当たり判定(群れ)
	/// </summary>
	/// <param name="bulletsList">プレイヤーの通常弾</param>
	/// <param name="enemys">敵(群れ)</param>
	static void checkBulletsEnemys(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys);
#pragma endregion 

#pragma region プレイヤーの通常弾と敵の弾の当たり判定
	/// <summary>
	/// プレイヤーの通常弾と敵の弾の当たり判定(単体)
	/// </summary>
	/// <param name="bullet">プレイヤーの通常弾</param>
	/// <param name="enemy">敵(単体)</param>
	static void checkBulletEnemybullet(bullet* bullet, Enemy* enemy);

	/// <summary>
	/// プレイヤーの通常弾と敵の弾の当たり判定(ボス)
	/// </summary>
	/// <param name="bulletsList">プレイヤーの通常弾</param>
	/// <param name="enemy">敵(単体)</param>
	static void checkBulletsEnemybullet(list<unique_ptr<bullet>>* bulletsList, Enemy* enemy);

	/// <summary>
	/// プレイヤーの通常弾と敵の弾の当たり判定(群れ)
	/// </summary>
	/// <param name="bulletsList">プレイヤーの通常弾</param>
	/// <param name="enemys">敵(群れ)</param>
	static void checkBulletsEnemyBullets(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys);
#pragma endregion 

#pragma region プレイヤーの追尾弾と敵本体の当たり判定
	/// <summary>
	/// プレイヤーの追尾弾と敵の当たり判定
	/// </summary>
	/// <param name="missilesList">プレイヤーの追尾弾</param>
	static void checkMissilesEnemy(list<unique_ptr<Missile>>* missilesList);
#pragma endregion

#pragma region プレイヤー本体と敵の弾の当たり判定
	/// <summary>
	/// プレイヤー本体と敵の弾の当たり判定(単体)
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="enemy">敵(単体)</param>
	static void chackPlayerEnemyBullet(Player* player, Enemy* enemy);

	/// <summary>
	/// プレイヤー本体と敵の弾の当たり判定(群れ)
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="enemys">敵(群れ)</param>
	static void checkPlayerEnemyBullets(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion

#pragma region プレイヤーと敵のロックオン判定
	/// <summary>
	/// プレイヤーカーソルと敵のロックオン判定(単体)
	/// </summary>
	/// <param name="player"></param>
	/// <param name="enemy"></param>
	static void checkRockonEnemy(Player* player, Enemy* enemy, int& targetnum);

	/// <summary>
	/// プレイヤーカーソルと敵のロックオン判定(群れ)
	/// </summary>
	/// <param name="player"></param>
	/// <param name="enemys"></param>
	static void checkRockonEnemys(Player* player, list<unique_ptr<Enemy>>* enemys, int& targetnum);
#pragma endregion

};