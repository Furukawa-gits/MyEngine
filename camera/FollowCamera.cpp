#include"FollowCamera.h"

void FollowCamera::setFollowTarget(XMFLOAT3* TargetPos, XMFLOAT3* TargetAngle, float TargetDis)
{
	this->TargetObjectPos = TargetPos;
	this->TargetObjectAngle = TargetAngle;
	this->TargetDistance = TargetDis;
}

void FollowCamera::Following()
{
	// �����_���W
	XMVECTOR targetPosition = XMLoadFloat3(TargetObjectPos);
	// ���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// �i���́j�����
	XMVECTOR upVector = XMLoadFloat3(&up);
}
