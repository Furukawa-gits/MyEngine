#pragma once
#include"Camera.h"


class FollowCamera
	:public Camera
{
public:
	FollowCamera();

	/// <summary>
	/// �Ǐ]����Ώۂ��Z�b�g
	/// </summary>
	/// <param name="TargetPos">�^�[�Q�b�g�̍��W</param>
	/// <param name="TargetAngle">�^�[�Q�b�g�̌���</param>
	/// <param name="TargetDis">�^�[�Q�b�g�܂ł̋���</param>
	void setFollowTarget(XMFLOAT3* TargetPos, XMFLOAT3* TargetAngle, float TargetDis);

	/// <summary>
	/// �Ǐ]����
	/// </summary>
	void Following();

	//�^�[�Q�b�g�̍��W
	XMFLOAT3* TargetObjectPos = nullptr;

	//�^�[�Q�b�g�̌���
	XMFLOAT3* TargetObjectAngle = nullptr;

	//�^�[�Q�b�g�Ƃ̋���
	float TargetDistance = 0.0f;
};
