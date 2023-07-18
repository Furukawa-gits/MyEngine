#pragma once
#include"Quaternion.h"
#include"RKDVector.h"
#include<vector>
#include <array>

class SplineCurve
{
public:
	SplineCurve();
	~SplineCurve();

	/// <summary>
	/// ���W���Z�b�g
	/// </summary>
	/// <param name="anyposition">����_(�n�_�ƏI�_������������)</param>
	/// <param name="allpointnum">����_�̐�</param>
	/// <param name="totalframe">���t���[��</param>
	void setSpline(RKDVector3* anyposition, int allpointnum, int totalframe);

	void play();

	RKDVector3 updata();

	void reset();

	//��Ԓ��̈�_���擾
	RKDVector3 getCurveSubdivisionPoint(float now);

private:

	//����_
	std::vector<RKDVector3> controlPoints;

	//���݈ʒu
	RKDVector3 returnPosition;

	//���݂̃��C������_
	int nowPlayIndex = 0;

	//�Đ��t���O
	bool isPlay = false;

	//���݃t���[���E���t���[��
	int nowFrame = 0;
	int totalFrame = 0;

	//�P�Ȑ�������̃t���[��(���t���[���Ɛ���_�̓s���㊄��؂�Ȃ��\��������̂�float�^)
	float oneCurveFrame = 0;

	//���t���[���ɑ΂��錻�݃t���[���̊���
	float nowTotalRate = 0;

	//�P�Ȑ�������̃t���[���ɑ΂��錻�݃t���[���̊���
	float nowOneCurveRate = 0;

	//�X�v���C���J�[�u�v�Z
	RKDVector3 splineCurve4(
		const RKDVector3& p0, 
		const RKDVector3& p1, 
		const RKDVector3& p2, 
		const RKDVector3& p3, float& t);
};