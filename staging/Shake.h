#pragma once

enum class shakeType
{
	vertical = 0,
	horizontal = 1,
	freely = 2
};

class Shake
{
public:
	Shake();
	~Shake();

	/// <summary>
	/// シェイクのセッティング
	/// </summary>
	/// <param name="type">シェイクタイプ(縦・横・縦横)</param>
	/// <param name="shakewide">シェイクの振れ幅</param>
	/// <param name="time">総フレーム数</param>
	void set(shakeType type, float shakewide, int time);

	/// <summary>
	/// シェイク
	/// </summary>
	float shaking();

	void shakeVertical();
	void shakehorizontal();
	void shakeFreely();

private:
	//有効化フラグ
	bool isActive = false;

	//シェイクタイプ
	shakeType thisType;

	//振れ幅(半径)
	float wide = 0;

	//総フレーム
	int totalFrame = 0;

	//返り値用の値
	float shakeReturn = 0;
};