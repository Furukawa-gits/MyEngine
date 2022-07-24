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

void FollowCamera::setFollowTarget(XMFLOAT3* TargetPos, XMFLOAT3* TargetAngle, float TargetDis)
{
	this->TargetObjectPos = TargetPos;
	this->TargetObjectAngle = TargetAngle;
	this->TargetDistance = TargetDis;
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

	/*XMVECTOR qCameraRightAngle = DirectX::XMQuaternionRotationAxis(upVector, XMConvertToRadians(rightAngle));
	XMFLOAT3 cameraSide;
	XMStoreFloat3(&cameraSide, XMVector3Normalize(XMVector3Cross(upVector, targetPosition - eyePosition)));
	XMVECTOR qCameraUpAngle = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&cameraSide), XMConvertToRadians(upAngle));

	XMVECTOR q = qCameraRightAngle * qCameraUpAngle;

	XMVECTOR qCameraPosition = { eye.x,eye.y,eye.z,1.0f };

	XMVECTOR qq = DirectX::XMQuaternionConjugate(q);

	qCameraPosition = q * qCameraPosition * qq;

	eye = { qCameraPosition.m128_f32[0],qCameraPosition.m128_f32[1],qCameraPosition.m128_f32[2] };

	XMVECTOR qCameraUp = { up.x,up.y,up.z,1.0f };
	qCameraUp = q * qCameraUp * qq;

	up = {
		qCameraUp.m128_f32[0],
		qCameraUp.m128_f32[1],
		qCameraUp.m128_f32[2]
	};*/

	//targetPosition = XMLoadFloat3(&target);
	//eyePosition = XMLoadFloat3(&eye);
	//upVector = XMLoadFloat3(&up);

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