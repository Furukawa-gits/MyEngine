#pragma once
#define _USE_MATH_DEFINES
#include<math.h>

enum class easingType
{
	easeIn = 0,
	easeOut = 1,
	easeInOut = 2
};

enum class easingPattern
{
	Quadratic = 0,
	Cubic = 1,
	Quartic = 2,
	Quintic = 3,
	Sinusoidal = 4,
	Exponential = 5,
	Circular = 6
};

//イージング
//実体を各々作って使うタイプ
class easingManager
{
public:
	easingManager();
	~easingManager();

	/// <summary>
	/// イージングのセット
	/// </summary>
	/// <param name="pattaern">イージングパターン</param>
	/// <param name="type">イージングタイプ</param>
	/// <param name="totaltime">イージング全体のフレーム</param>
	/// <param name="start">初期位置</param>
	/// <param name="end">最終位置</param>
	void set(easingType pattaern, easingPattern type, int totaltime, float start, float end);

	/// <summary>
	/// <para>イージング</para>
	/// 例：オブジェクトの座標 = 返り値
	/// </summary>
	/// <returns>現在のイージング進行度(割合)</returns>
	float easing();

	/// <summary>
	/// 動いているかどうかを取得
	/// </summary>
	bool getIsActive() { return isActive; }

	void reSet() { isActive = false; }

private:
	//イーズイン
	void easeIn();

	//イーズアウト
	void easeOut();

	//イーズインアウト
	void easeInOut();

	void inQuadratic();
	void inCubic();
	void inQuartic();
	void inQuintic();
	void inSinusoidal();
	void inExponential();
	void inCircular();

	void outQuadratic();
	void outCubic();
	void outQuartic();
	void outQuintic();
	void outSinusoidal();
	void outExponential();
	void outCircular();

	void inOutQuadratic();
	void inOutCubic();
	void inOutQuartic();
	void inOutQuintic();
	void inOutSinusoidal();
	void inOutExponential();
	void inOutCircular();


private:
	//有効化フラグ
	bool isActive = false;

	//現在の進行度(フレーム)
	int frame = 0;

	//イージング全体にかかる時間(フレーム)
	int totalFrame = 0;

	//現在のイージングの割合(この値と座標や角度を使ってオブジェクトを動かす)
	float rate = 0.0f;

	//初期値
	float start = 0;

	//移動量
	float lange = 0;

	//イージングの値(返り値用)
	float easingNum = 0;

	//イージングの種類
	easingType thisType;

	//イージングのレベル
	easingPattern thisPattern;
};