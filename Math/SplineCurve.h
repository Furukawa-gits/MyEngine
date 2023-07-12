#pragma once
#include"../FbxLoder/Quaternion.h"
#include<vector>

class SplineCurve
{
public:
	SplineCurve();
	~SplineCurve();

	void setSpline(XMFLOAT3* anyposition, int allpointnum, int totalframe);

	void play();

	XMFLOAT3 updata();

	void reset();

	std::vector<XMFLOAT3> getCurveSubdivisionPoint();

private:

	//����_
	std::vector<XMFLOAT3> controlPoints;

	//���݈ʒu
	XMFLOAT3 returnPosition;

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
	float nowFramePerTotalFrame = 0;

	//�P�Ȑ�������̃t���[���ɑ΂��錻�݃t���[���̊���
	float nowFramePerOneCurveFrame = 0;

	//�X�v���C���J�[�u�v�Z
	XMFLOAT3 splineCurve4(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3);
};