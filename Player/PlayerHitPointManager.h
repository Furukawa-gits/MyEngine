#pragma once
#include"../2D/Sprite.h"

//プレイヤーのHPマネージャー
class PlayerHitPointManager
{
public:
	PlayerHitPointManager();
	~PlayerHitPointManager();

	//リソース読み込み
	void loadSprites();

	//ダメージを受ける
	void Damage(int damage);

	//パラメータリセット
	void reSet();

	//更新
	void updata(int* isinvisible);

	//UI描画
	void drawHPUI(directX* directx);

	bool getIsAlive() { return isAlive; }

	bool getisArmor() { return isArmor; }

	int getPlayerHP() { return playerHP; }

private:
	//生存フラグ
	bool isAlive;

	//HP
	int playerHP;
	const int maxHP = 10;

	//HPが危険になった時の警告
	bool isDangerHP = false;
	int cautionHPCount = 0;

	//無敵時間フラグ
	bool isArmor = false;
	int armorTime = 0;
	const int maxArmorTime = 300;

	//HPゲージ
	SingleSprite HPGaugeBar;
	SingleSprite dangarHPGaugeBar;

	//ダメージ表現
	SingleSprite damage;
};