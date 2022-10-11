#include"FollowCamera.h"
#include"../Base/WindowGenerate.h"

FollowCamera::FollowCamera()
{
	aspectRatio = (float)win_width / win_hight;

	//ビュー行列の計算
	UpdateViewMatrix();

	// 射影行列の計算
	UpdateProjectionMatrix();

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}

void FollowCamera::setFollowTarget(XMFLOAT3 TargetPos,XMFLOAT3 TargetAngle, const float TargetDis)
{
	TargetObjectPos = &TargetPos;
	TargetObjectAngle = &TargetAngle;
	TargetDistance = TargetDis;
	target = *TargetObjectPos;
	eye = { target.x,target.y,target.z + TargetDistance };
	up = { 0,1,0 };
}

void FollowCamera::Following(XMFLOAT3 upVector, XMFLOAT3 forwordVector, XMFLOAT3 targetPos)
{
	target = targetPos;

	XMVECTOR targetPosition = XMLoadFloat3(&target);

	XMFLOAT3 backVector = { -forwordVector.x,-forwordVector.y ,-forwordVector.z };

	float backLength = sqrtf(powf(backVector.x, 2) + powf(backVector.y, 2) + powf(backVector.z, 2));

	backLength = 1 / backLength;

	backVector = {
		backVector.x * backLength * TargetDistance,
		backVector.y * backLength * TargetDistance,
		backVector.z * backLength * TargetDistance
	};

	XMVECTOR eyePosition = targetPosition + XMLoadFloat3(&backVector);

	matView = XMMatrixLookAtLH(eyePosition, targetPosition, XMLoadFloat3(&upVector));

	UpdateProjectionMatrix();

	matViewProjection = matView * matProjection;

	XMStoreFloat3(&target, targetPosition);
	XMStoreFloat3(&eye, eyePosition);
	XMStoreFloat3(&up, XMLoadFloat3(&upVector));

	Update();

	frontVec = {
		target.x - eye.x,
		target.y - eye.y,
		target.z - eye.z
	};

	float length = sqrtf(powf(frontVec.x, 2) + powf(frontVec.y, 2) + powf(frontVec.z, 2));

	length = 1 / length;

	frontVec = {
		frontVec.x * length,
		frontVec.y * length,
		frontVec.z * length
	};
}