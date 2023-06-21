#pragma once
#include"../2D/Sprite.h"

//�v���C���[��HP�}�l�[�W���[
class PlayerHitPointManager
{
public:
	PlayerHitPointManager();
	~PlayerHitPointManager();

	//���\�[�X�ǂݍ���
	void loadSprites();

	//�_���[�W���󂯂�
	void Damage(int damage);

	//�p�����[�^���Z�b�g
	void reSet();

	//�X�V
	void updata(int* isinvisible);

	//UI�`��
	void drawHPUI(directX* directx);

	bool getIsAlive() { return isAlive; }

	bool getisArmor() { return isArmor; }

	int getPlayerHP() { return playerHP; }

private:
	//�����t���O
	bool isAlive;

	//HP
	int playerHP;
	const int maxHP = 10;

	//HP���댯�ɂȂ������̌x��
	bool isDangerHP = false;
	int cautionHPCount = 0;

	//���G���ԃt���O
	bool isArmor = false;
	int armorTime = 0;
	const int maxArmorTime = 300;

	//HP�Q�[�W
	SingleSprite HPGaugeBar;
	SingleSprite dangarHPGaugeBar;

	//�_���[�W�\��
	SingleSprite damage;
};