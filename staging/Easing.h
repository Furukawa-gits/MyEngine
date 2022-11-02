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

//�C�[�W���O
//���̂��e�X����Ďg���^�C�v
class easingManager
{
public:
	easingManager();
	~easingManager();

	//�C�[�W���O�̃Z�b�g
	void set(easingPattern pattaern, easingLevel level, int totaltime, float start, float end);

	/// <summary>
	/// <para>�C�[�W���O</para>
	/// ��F�I�u�W�F�N�g�̍��W = �Ԃ�l
	/// </summary>
	/// <returns>���݂̃C�[�W���O�i�s�x(����)</returns>
	float easing();

private:
	//�C�[�Y�C��
	void easeIn();

	//�C�[�Y�A�E�g
	void easeOut();

	//�C�[�Y�C���A�E�g
	void easeInOut();

	void inQuadratic();

	void inCubic();

	void inQuartic();

	void inQuintic();

	void inSinusoidal();

	void inExponential();

	void inCircular();


private:
	//�L�����t���O
	bool isActive = false;

	//���݂̐i�s�x(�t���[��)
	int frame = 0;

	//�C�[�W���O�S�̂ɂ����鎞��(�t���[��)
	int totalFrame = 0;

	//���݂̃C�[�W���O�̊���(���̒l�ƍ��W��p�x���g���ăI�u�W�F�N�g�𓮂���)
	float rate = 0.0f;

	//�����l
	float start = 0;

	//�ŏI�l
	float end = 0;

	//�C�[�W���O�̒l(�Ԃ�l�p)
	float easingNum = 0;

	//�C�[�W���O�̎��
	easingPattern thisPattern;

	//�C�[�W���O�̃��x��
	easingLevel thisLevel;
};