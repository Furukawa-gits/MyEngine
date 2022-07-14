#include"FollowCamera.h"

void FollowCamera::setFollowTarget(XMFLOAT3* TargetPos, XMFLOAT3* TargetAngle, float TargetDis)
{
	this->TargetObjectPos = TargetPos;
	this->TargetObjectAngle = TargetAngle;
	this->TargetDistance = TargetDis;
}

void FollowCamera::Following()
{
	//SetEye(*TargetObjectPos);
	SetTarget(*TargetObjectPos);
}
