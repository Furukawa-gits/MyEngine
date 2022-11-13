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

//�C�[�W���O
//���̂��e�X����Ďg���^�C�v
class easingManager
{
public:
	easingManager();
	~easingManager();

	/// <summary>
	/// �C�[�W���O�̃Z�b�g
	/// </summary>
	/// <param name="pattaern">�C�[�W���O�p�^�[��</param>
	/// <param name="type">�C�[�W���O�^�C�v</param>
	/// <param name="totaltime">�C�[�W���O�S�̂̃t���[��</param>
	/// <param name="start">�����ʒu</param>
	/// <param name="end">�ŏI�ʒu</param>
	void set(easingType pattaern, easingPattern type, int totaltime, float start, float end);

	/// <summary>
	/// <para>�C�[�W���O</para>
	/// ��F�I�u�W�F�N�g�̍��W = �Ԃ�l
	/// </summary>
	/// <returns>���݂̃C�[�W���O�i�s�x(����)</returns>
	float easing();

	/// <summary>
	/// �����Ă��邩�ǂ������擾
	/// </summary>
	bool getIsActive() { return isActive; }

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

	//�ړ���
	float lange = 0;

	//�C�[�W���O�̒l(�Ԃ�l�p)
	float easingNum = 0;

	//�C�[�W���O�̎��
	easingType thisType;

	//�C�[�W���O�̃��x��
	easingPattern thisPattern;
};