#pragma once
#include"Player.h"

using namespace std;

//����Ă邱��
//�v���C���[�ƓG�{�̂̓����蔻��
//�v���C���[�̒ʏ�e�ƓG�̓����蔻��
//�v���C���[�̒ǔ��e�ƓG�̓����蔻��
//�v���C���[�ƓG�̒e�̓����蔻��
//�v���C���[�ƓG�̃��b�N�I������

/// <summary>
/// �v���C���[�ƓG�{�̂̓����蔻��(�P��)
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="enemy">�G(�P��)</param>
void checkPlayerEnemy(Player* player,Enemy* enemy);

/// <summary>
/// �v���C���[�ƓG�{�̂̓����蔻��(�Q��)
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="enemys">�G(�Q��)</param>
void checkPlayerEnemys(Player* player, vector<Enemy>* enemys);

/// <summary>
/// �v���C���[�̒ʏ�e�ƓG�̓����蔻��(�P��)
/// </summary>
/// <param name="bullets">�v���C���[�̒ʏ�e</param>
/// <param name="enemy">�G(�P��)</param>
void checkBulletEnemy(vector<bullet>* bullets, Enemy* enemy);

/// <summary>
/// �v���C���[�̒ʏ�e�ƓG�̓����蔻��(�Q��)
/// </summary>
/// <param name="bullets">�v���C���[�̒ʏ�e</param>
/// <param name="enemys">�G(�Q��)</param>
void checkBulletEnemy(vector<bullet>* bullets, vector<Enemy>* enemys);

/// <summary>
/// �v���C���[�̒ǔ��e�ƓG�̓����蔻��
/// </summary>
/// <param name="missiles">�v���C���[�̒ǔ��e</param>
void checkHomingEnemy(vector<Missile>* missiles);

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
void checkPlayerEnemyBullets(Player* player, vector<Enemy>* enemys);

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
void checkRockonEnemys(Player* player, vector<Enemy>* enemys);