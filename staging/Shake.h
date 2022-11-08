#pragma once
#include<time.h>
#include<random>

/// <summary>
/// 
/// </summary>
class Shake
{
public:
	Shake();
	~Shake();

	/// <summary>
	/// シェイクのセッティング
	/// </summary>
	/// <param name="shakewide">シェイクの振れ幅</param>
	/// <param name="totalTime">総フレーム数</param>
	void set(int shakewide, int totalTime);

	/// <summary>
	/// シェイク
	/// </summary>
	int shaking();

private:
	//有効化フラグ
	bool isActive = false;

	//振れ幅(半径)
	int wide = 0;

	//総フレーム
	int totalFrame = 0;

	//返り値用の値
	int shakeReturn = 0;
};