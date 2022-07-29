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
	/// �t���[���J�E���g���Z
	/// </summary>
	static void easingUpdate();

	/// <summary>
	/// ���݂̃t���[�����������t���[���ɃZ�b�g
	/// </summary>
	void easingSet();

	/// <summary>
	/// �C�[�Y�C��(Quad)
	/// </summary>
	/// <param name="start">�����ʒu</param>
	/// <param name="end">�ŏI�ʒu</param>
	/// <param name="totaltime">���t���[��</param>
	float easeInQuad(float start, float end, float totaltime);

	/// <summary>
	/// �C�[�Y�A�E�g(Quad)
	/// </summary>
	/// <param name="start">�����ʒu</param>
	/// <param name="end">�ŏI�ʒu</param>
	/// <param name="totaltime">���t���[��</param>
	float easeOutQuad(float start, float end, float totaltime);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="start">�����ʒu</param>
	/// <param name="end">�ŏI�ʒu</param>
	/// <param name="totaltime">���t���[��</param>
	float easeInOutQuad(float start, float end, float totaltime);
};