#pragma once

class easing
{
private:
	static float time;
	float starttime = 0.0f;

public:
	bool isStart = false;

public:

	/// <summary>
	/// フレームカウント加算
	/// </summary>
	static void easingUpdate();

	/// <summary>
	/// 現在のフレーム数を初期フレームにセット
	/// </summary>
	void easingSet();

	/// <summary>
	/// イーズイン(Quad)
	/// </summary>
	/// <param name="start">初期位置</param>
	/// <param name="end">最終位置</param>
	/// <param name="totaltime">総フレーム</param>
	float easeInQuad(float start, float end, float totaltime);

	/// <summary>
	/// イーズアウト(Quad)
	/// </summary>
	/// <param name="start">初期位置</param>
	/// <param name="end">最終位置</param>
	/// <param name="totaltime">総フレーム</param>
	float easeOutQuad(float start, float end, float totaltime);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="start">初期位置</param>
	/// <param name="end">最終位置</param>
	/// <param name="totaltime">総フレーム</param>
	float easeInOutQuad(float start, float end, float totaltime);
};