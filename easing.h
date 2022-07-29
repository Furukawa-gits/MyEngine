#pragma once

class easing
{
	static float time;

	float start = 0.0f;
	float end = 0.0f;
	float starttime;
	bool isStart = false;

public:
	void easingInit();

	void easingUpdate();

	void easingSet(float start,float end);
};