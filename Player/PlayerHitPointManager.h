#pragma once
#include"../2D/Sprite.h"

//プレイヤーのHPマネージャー
class PlayerHitPointManager
{
public:
	PlayerHitPointManager();
	~PlayerHitPointManager();

	//初期化
	void init();

	//リソース読み込み
	void loadSprites();

	//ダメージを受ける
	void Damage(int damage);

	//リセット
	void reSet();

	//更新
	void updata(int* isinvisible);

	//UI描画
	void drawHPUI(directX* directx);

	bool getIsAlive() { return isAlive; }

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