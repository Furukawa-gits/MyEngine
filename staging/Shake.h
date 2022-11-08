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
	/// �V�F�C�N�̃Z�b�e�B���O
	/// </summary>
	/// <param name="type">�V�F�C�N�^�C�v(�c�E���E�c��)</param>
	/// <param name="shakewide">�V�F�C�N�̐U�ꕝ</param>
	/// <param name="time">���t���[����</param>
	void set(shakeType type, float shakewide, int time);

	/// <summary>
	/// �V�F�C�N
	/// </summary>
	float shaking();

	void shakeVertical();
	void shakehorizontal();
	void shakeFreely();

private:
	//�L�����t���O
	bool isActive = false;

	//�V�F�C�N�^�C�v
	shakeType thisType;

	//�U�ꕝ(���a)
	float wide = 0;

	//���t���[��
	int totalFrame = 0;

	//�Ԃ�l�p�̒l
	float shakeReturn = 0;
};