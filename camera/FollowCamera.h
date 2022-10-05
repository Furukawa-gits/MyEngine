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
	/// �J�����̍��W���F�N�H�[�^�j�I���̉�]�����Z�b�g
	/// �J�����̍��W�F�O�����̉�]������t�Z
	/// </summary>
	/// <param name="upVector">������̉�]��</param>
	/// <param name="forwordVector">�O�����̉�]��</param>
	void Following();

	void setAngle(float up, float right)
	{
		this->upAngle = up;
		this->rightAngle = right;
	}

	void setFrontVec(float speed) {
		frontVec.x *= speed;
		frontVec.y *= speed;
		frontVec.z *= speed;
	}

	XMFLOAT3 getFrontVec() { return frontVec; }

	//�^�[�Q�b�g�̍��W
	XMFLOAT3* TargetObjectPos = nullptr;

	//�^�[�Q�b�g�̌���
	XMFLOAT3* TargetObjectAngle = nullptr;

	//�^�[�Q�b�g�Ƃ̋���
	float TargetDistance = 0.0f;

private:

	float upAngle = 0.0f;
	float rightAngle = 0.0f;

	//�i�s�����x�N�g��
	XMFLOAT3 frontVec = { 0,0,0 };
};
