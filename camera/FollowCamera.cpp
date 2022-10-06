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

void FollowCamera::setFollowTarget(const XMFLOAT3* TargetPos, const XMFLOAT3* TargetAngle, const float TargetDis)
{
	*TargetObjectPos = *TargetPos;
	*TargetObjectAngle = *TargetAngle;
	TargetDistance = TargetDis;
	target = *TargetObjectPos;
	eye = { target.x,target.y,target.z + TargetDistance };
	up = { 0,1,0 };
}

void FollowCamera::Following()
{
	target = *TargetObjectPos;

	XMVECTOR targetPosition = XMLoadFloat3(&target);

	XMVECTOR noCalcVec = { 0,0,-TargetDistance,0 };

	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationZ(XMConvertToRadians(TargetObjectAngle->z + 180));
	rotM *= XMMatrixRotationX(XMConvertToRadians(-(TargetObjectAngle->x)));
	rotM *= XMMatrixRotationY(XMConvertToRadians(TargetObjectAngle->y + 180.0f));

	XMVECTOR eyePosition = targetPosition + XMVector3Transform(noCalcVec, rotM);

	XMVECTOR upVector = XMLoadFloat3(&up);

	matView = XMMatrixLookAtLH(eyePosition, targetPosition, upVector);

	UpdateProjectionMatrix();

	matViewProjection = matView * matProjection;

	XMStoreFloat3(&target, targetPosition);
	XMStoreFloat3(&eye, eyePosition);
	XMStoreFloat3(&up, upVector);

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