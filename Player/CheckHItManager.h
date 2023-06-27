#pragma once
#include<memory>
#include<list>
#define _USE_MATH_DEFINES
#include<math.h>
#include"Player.h"
#include"Enemy.h"
#include"HomingMissile.h"
#include"uniteBoss.h"

using namespace std;

//����Ă邱�ƈꗗ
//�v���C���[�ƓG�{�̂̓����蔻��
//�v���C���[�̒ʏ�e�ƓG�̓����蔻��
//�v���C���[�̒ǔ��e�ƓG�̓����蔻��
//�v���C���[�ƓG�̒e�̓����蔻��
//�v���C���[�ƓG�̃��b�N�I������

class checkHitManager
{
public:

#pragma region �v���C���[�{�̂ƓG�{�̂̓����蔻��
	/// <summary>
	/// �v���C���[�{�̂ƓG�{�̂̓����蔻��(�P��)
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="enemy">�G(�P��)</param>
	static void checkPlayerEnemy(Player* player, Enemy* enemy);

	/// <summary>
	/// �v���C���[�{�̂ƓG�{�̂̓����蔻��(�Q��)
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="enemys">�G(�Q��)</param>
	static void checkPlayerEnemys(Player* player, list<unique_ptr<Enemy>>* enemys);
#pragma endregion

#pragma region �v���C���[�̒ʏ�e�ƓG�{�̂̓����蔻��
	/// <summary>
	/// �v���C���[�̒ʏ�e�ƓG�{�̂̓����蔻��(�P��)
	/// </summary>
	/// <param name="bullet">�v���C���[�̒ʏ�e</param>
	/// <param name="enemy">�G(�P��)</param>
	static void checkBulletEnemy(NormalBullet* bullet, Enemy* enemy);

	/// <summary>
	/// �v���C���[�̒ʏ�e�ƓG�̓����蔻��(�{�X)
	/// </summary>
	/// <param name="bulletsList">�v���C���[�̒ʏ�e</param>
	/// <param name="enemy">�G(�P��)</param>
	static void checkBulletsEnemy(list<unique_ptr<NormalBullet>>* bulletsList, Enemy* enemy);

	/// <summary>
	/// �v���C���[�̒ʏ�e�ƓG�{�̂̓����蔻��(�Q��)
	/// </summary>
	/// <param name="bulletsList">�v���C���[�̒ʏ�e</param>
	/// <param name="enemys">�G(�Q��)</param>
	static void checkBulletsEnemys(list<unique_ptr<NormalBullet>>* bulletsList, list<unique_ptr<Enemy>>* enemys);
#pragma endregion 

#pragma region �v���C���[�̒ǔ��e�ƓG�{�̂̓����蔻��
	/// <summary>
	/// �v���C���[�̒ǔ��e�ƓG�̓����蔻��
	/// </summary>
	/// <param name="missilesList">�v���C���[�̒ǔ��e</param>
	static void checkMissilesEnemy(list<unique_ptr<Missile>>* missilesList);
#pragma endregion

#pragma region �v���C���[�̒ʏ�e�ƓG�̗��˒e�̓����蔻��
	/// <summary>
	/// �v���C���[�̒ʏ�e�ƓG�̗��˒e�̓����蔻��(�P�̓��m)
	/// </summary>
	/// <param name="playerbullet">�v���C���[�̒ʏ�e(�P��)</param>
	/// <param name="rampagebullet">�G�̗��˒e(�P��)</param>
	static void checkBulletEnemyRampage(NormalBullet* playerbullet,NormalBullet* rampagebullet);

	/// <summary>
	/// �v���C���[�̒ʏ�e�ƓG�̗��˒e�̓����蔻��(list���m)
	/// </summary>
	/// <param name="bulletsList">�v���C���[�̒ʏ�e(list)</param>
	/// <param name="rampagebullets">�G�̗��˒e(list)</param>
	static void checkBulletsEnemyRampage(list<unique_ptr<NormalBullet>>* bulletsList, Enemy *enemy);
#pragma endregion

#pragma region �v���C���[�{�̂ƓG�̗��˒e�̓����蔻��
	/// <summary>
	/// �v���C���[�{�̂ƓG�̗��˒e�̓����蔻��(�P��)
	/// </summary>
	/// <param name="player">�v���C���[�{��</param>
	/// <param name="rampagebullet">�G�̗��˒e</param>
	static void checkPlayerEnemyRampage(Player* player, NormalBullet* rampagebullet);

	/// <summary>
	/// �v���C���[�{�̂ƓG�̗��˒e�̓����蔻��(���X�g)
	/// </summary>
	/// <param name="player">�v���C���[�{��</param>
	/// <param name="rampagebullets">�G�̗��˒e</param>
	static void checkPlayerEnemyRampages(Player* player, Enemy* enemy);
#pragma endregion

#pragma region �v���C���[�ƓG�̃��b�N�I������
	/// <summary>
	/// �v���C���[�J�[�\���ƓG�̃��b�N�I������(�P��)
	/// </summary>
	/// <param name="player"></param>
	/// <param name="enemy"></param>
	static void checkRockonEnemy(Player* player, Enemy* enemy, int& targetnum);

	/// <summary>
	/// �v���C���[�J�[�\���ƓG�̃��b�N�I������(�Q��)
	/// </summary>
	/// <param name="player"></param>
	/// <param name="enemys"></param>
	static void checkRockonEnemys(Player* player, list<unique_ptr<Enemy>>* enemys, int& targetnum);
#pragma endregion

};