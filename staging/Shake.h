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
	/// �V�F�C�N�̃Z�b�e�B���O
	/// </summary>
	/// <param name="shakewide">�V�F�C�N�̐U�ꕝ</param>
	/// <param name="totalTime">���t���[����</param>
	void set(int shakewide, int totalTime);

	/// <summary>
	/// �V�F�C�N
	/// </summary>
	int shaking();

private:
	//�L�����t���O
	bool isActive = false;

	//�U�ꕝ(���a)
	int wide = 0;

	//���t���[��
	int totalFrame = 0;

	//�Ԃ�l�p�̒l
	int shakeReturn = 0;
};