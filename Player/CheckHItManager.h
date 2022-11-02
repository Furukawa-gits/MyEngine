#pragma once
#include"Player.h"

using namespace std;

//やってること
//プレイヤーと敵本体の当たり判定
//プレイヤーの通常弾と敵の当たり判定
//プレイヤーの追尾弾と敵の当たり判定
//プレイヤーと敵の弾の当たり判定
//プレイヤーと敵のロックオン判定

/// <summary>
/// プレイヤーと敵本体の当たり判定(単体)
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="enemy">敵(単体)</param>
void checkPlayerEnemy(Player* player,Enemy* enemy);

/// <summary>
/// プレイヤーと敵本体の当たり判定(群れ)
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="enemys">敵(群れ)</param>
void checkPlayerEnemys(Player* player, vector<Enemy>* enemys);

/// <summary>
/// プレイヤーの通常弾と敵の当たり判定(単体)
/// </summary>
/// <param name="bullets">プレイヤーの通常弾</param>
/// <param name="enemy">敵(単体)</param>
void checkBulletEnemy(vector<bullet>* bullets, Enemy* enemy);

/// <summary>
/// プレイヤーの通常弾と敵の当たり判定(群れ)
/// </summary>
/// <param name="bullets">プレイヤーの通常弾</param>
/// <param name="enemys">敵(群れ)</param>
void checkBulletEnemy(vector<bullet>* bullets, vector<Enemy>* enemys);

/// <summary>
/// プレイヤーの追尾弾と敵の当たり判定
/// </summary>
/// <param name="missiles">プレイヤーの追尾弾</param>
void checkHomingEnemy(vector<Missile>* missiles);

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
void checkPlayerEnemyBullets(Player* player, vector<Enemy>* enemys);

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
void checkRockonEnemys(Player* player, vector<Enemy>* enemys);