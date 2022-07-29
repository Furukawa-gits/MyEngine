#include"easing.h"

float easing::time = 0.0f;

void easing::easingUpdate()
{
	time++;
}

void easing::easingSet()
{
	starttime = time;
	isStart = true;
}

float easing::easeInQuad(float start, float end, float totaltime)
{
	//�X�^�[�g�t���O�������Ă��Ȃ���΃X�^�[�g���Ȃ�
	if (isStart)
	{
		//����
		float t = (time - starttime) / totaltime;
		//start��end�̋���
		float dis = end - start;

		if (t > 1.0f)
		{
			return end;
		}

		return dis * t * t + start;
	}

	return start;
}

float easing::easeOutQuad(float start, float end, float totaltime)
{
	//�X�^�[�g�t���O�������Ă��Ȃ���΃X�^�[�g���Ȃ�
	if (isStart)
	{
		//����
		float t = (time - starttime) / totaltime;
		//start��end�̋���
		float dis = end - start;

		if (t > 1.0f)
		{
			return end;
		}

		return -dis * t * (t - 2.0f) + start;
	}

	return start;
}

float easing::easeInOutQuad(float start, float end, float totaltime)
{
	//�X�^�[�g�t���O�������Ă��Ȃ���΃X�^�[�g���Ȃ�
	if (isStart)
	{
		//����
		float t = (time - starttime) / totaltime;
		//start��end�̋���
		float dis = end - start;

		//�O����
		if (t < 1.0f)
		{
			return dis / 2.0f * t * t + start;
		}
		else
		{
			t--;
			if (t > 1.0f)
			{
				return end;
			}
			return -dis / 2.0f * (t * (t - 2.0f) - 1.0f) + start;
		}

	}

	return start;
}
