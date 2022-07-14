#include"FollowCamera.h"

void FollowCamera::setFollowTarget(XMFLOAT3* TargetPos, XMFLOAT3* TargetAngle, float TargetDis)
{
	this->TargetObjectPos = TargetPos;
	this->TargetObjectAngle = TargetAngle;
	this->TargetDistance = TargetDis;
}

void FollowCamera::Following()
{
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(TargetObjectPos);
	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// （仮の）上方向
	XMVECTOR upVector = XMLoadFloat3(&up);
}
