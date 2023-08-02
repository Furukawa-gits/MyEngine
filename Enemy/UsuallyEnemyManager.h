#pragma once
#include"TutorialEnemy.h"
#include"ChaseEnemy.h"
#include"ShotEnemy.h"
#include"HomingEnemy.h"
#include"RampageEnemy.h"


class UsuallyEnemymanager
{
public:
	UsuallyEnemymanager();
	~UsuallyEnemymanager();

	void init();

	/// <summary>
	/// �G�ǉ�
	/// <para>0:�`���[�g���A��</para>
	/// <para>1:�ǐՂ̂�</para>
	/// <para>2:�P���ˌ�</para>
	/// <para>3:�ǐՁ��ˌ�</para>
	/// <para>4:���ˌ�</para>
	/// <para>10:�ʏ�{�X</para>
	/// <para>100:���j�b�g�{�X</para>
	/// </summary>
	/// <param name="addtype">�G�^�C�v</param>
	void addUsuallyEnemy(enemyType addtype);

	void update();

	void draw3D();

	void draw2D();

	void drawUIs();

	std::list<unique_ptr<primitiveEnemy>>& getEnemyList() { return enemyList; }

private:
	std::list<unique_ptr<primitiveEnemy>> enemyList;

};