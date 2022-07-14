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
	eye = { target.x,target.y,target.z - TargetDistance };
	up = { 0,1,0 };
}

void FollowCamera::Following()
{
	target = *TargetObjectPos;

	eye = { target.x,target.y,target.z - TargetDistance };

	Update();
}