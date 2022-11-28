#pragma once
#include<memory>
#include<list>
#include"Player.h"
#include"Enemy.h"
#include"Bullets.h"

using namespace std;

//����Ă邱�ƈꗗ
//�v���C���[�ƓG�{�̂̓����蔻��
//�v���C���[�̒ʏ�e�ƓG�̓����蔻��
//�v���C���[�̒ǔ��e�ƓG�̓����蔻��
//�v���C���[�ƓG�̒e�̓����蔻��
//�v���C���[�ƓG�̃��b�N�I������

#pragma region �v���C���[�{�̂ƓG�{�̂̓����蔻��
/// <summary>
/// �v���C���[�ƓG�{�̂̓����蔻��(�P��)
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="enemy">�G(�P��)</param>
void checkPlayerEnemy(Player* player, Enemy* enemy);

/// <summary>
/// �v���C���[�ƓG�{�̂̓����蔻��(�Q��)
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="enemys">�G(�Q��)</param>
void checkPlayerEnemys(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion

#pragma region �v���C���[�̒ʏ�e�ƓG�{�̂̓����蔻��
/// <summary>
/// �v���C���[�̒ʏ�e�ƓG�̓����蔻��(�P��)
/// </summary>
/// <param name="bulletsList">�v���C���[�̒ʏ�e</param>
/// <param name="enemy">�G(�P��)</param>
void checkBulletEnemy(bullet* bullet, Enemy* enemy);

/// <summary>
/// �v���C���[�̒ʏ�e�ƓG�̓����蔻��(�Q��)
/// </summary>
/// <param name="bulletsList">�v���C���[�̒ʏ�e</param>
/// <param name="enemys">�G(�Q��)</param>
void checkBulletEnemys(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys);
#pragma endregion 

#pragma region �v���C���[�̒ʏ�e�ƓG�̒e�̓����蔻��
/// <summary>
/// �v���C���[�̒ʏ�e�ƓG�̓����蔻��(�P��)
/// </summary>
/// <param name="bulletsList">�v���C���[�̒ʏ�e</param>
/// <param name="enemy">�G(�P��)</param>
void checkBulletEnemybullet(bullet* bullet, Enemy* enemy);

/// <summary>
/// �v���C���[�̒ʏ�e�ƓG�̓����蔻��(�Q��)
/// </summary>
/// <param name="bulletsList">�v���C���[�̒ʏ�e</param>
/// <param name="enemys">�G(�Q��)</param>
void checkBulletEnemyBullets(list<unique_ptr<bullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys);
#pragma endregion 

#pragma region �v���C���[�̒ǔ��e�ƓG�{�̂̓����蔻��
/// <summary>
/// �v���C���[�̒ǔ��e�ƓG�̓����蔻��
/// </summary>
/// <param name="missilesList">�v���C���[�̒ǔ��e</param>
void checkHomingEnemy(list<unique_ptr<Missile>>* missilesList);
#pragma endregion

#pragma region �v���C���[�{�̂ƓG�̒e�̓����蔻��
/// <summary>
/// �v���C���[�ƓG�̒e�̓����蔻��(�P��)
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="enemy">�G(�P��)</param>
void chackPlayerEnemyBullet(Player* player, Enemy* enemy);

/// <summary>
/// �v���C���[�ƓG�̒e�̓����蔻��(�Q��)
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="enemys">�G(�Q��)</param>
void checkPlayerEnemyBullets(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion

#pragma region �v���C���[�ƓG�̃��b�N�I������
/// <summary>
/// �v���C���[�ƓG�̃��b�N�I������(�P��)
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
void checkRockonEnemy(Player* player, Enemy* enemy);

/// <summary>
/// �v���C���[�ƓG�̃��b�N�I������(�Q��)
/// </summary>
/// <param name="player"></param>
/// <param name="enemys"></param>
void checkRockonEnemys(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion