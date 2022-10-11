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
	void setFollowTarget(XMFLOAT3 TargetPos, XMFLOAT3 TargetAngle, const float TargetDis);

	/// <summary>
	/// �Ǐ]����
	/// �J�����̍��W���F�N�H�[�^�j�I���̉�]�����Z�b�g
	/// �J�����̍��W�F�O�����̉�]������t�Z
	/// </summary>
	/// <param name="upVector">������̉�]��</param>
	/// <param name="forwordVector">�O�����̉�]��</param>
	/// <param name="targetPos">�Ǐ]����Ώۂ̍��W</param>
	void Following(XMFLOAT3 upVector, XMFLOAT3 forwordVector, XMFLOAT3 targetPos);

	void setAngle(float up, float right)
	{
		/*this->upAngle = up;
		this->rightAngle = right;*/
	}

	void setFrontVec(const float speed) {
		frontVec.x *= speed;
		frontVec.y *= speed;
		frontVec.z *= speed;
	}

	void setTargets(const XMFLOAT3 pos, const XMFLOAT3 angle)
	{
		*TargetObjectPos = pos;
		*TargetObjectAngle = angle;
	}

	XMFLOAT3 getFrontVec() { return frontVec; }

private:

	//�^�[�Q�b�g�̍��W
	XMFLOAT3* TargetObjectPos = nullptr;

	//�^�[�Q�b�g�̌���
	XMFLOAT3* TargetObjectAngle = nullptr;

	//�^�[�Q�b�g�Ƃ̋���
	float TargetDistance = 0.0f;

	//�i�s�����x�N�g��
	XMFLOAT3 frontVec = { 0,0,0 };
};
