#pragma once

enum class easingPattern
{
	easeIn = 0,
	easeOut = 1,
	easeInOut = 2
};

enum class easingLevel
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

	//イージングのセット
	void set(easingPattern pattaern, easingLevel level, int totaltime, float start, float end);

	/// <summary>
	/// <para>イージング</para>
	/// 例：オブジェクトの座標 = 返り値
	/// </summary>
	/// <returns>現在のイージング進行度(割合)</returns>
	float easing();

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

	//最終値
	float end = 0;

	//イージングの値(返り値用)
	float easingNum = 0;

	//イージングの種類
	easingPattern thisPattern;

	//イージングのレベル
	easingLevel thisLevel;
};