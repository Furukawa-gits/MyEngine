#pragma once
#include"Camera.h"


class FollowCamera
	:public Camera
{
public:
	FollowCamera();

	/// <summary>
	/// 追従する対象をセット
	/// </summary>
	/// <param name="TargetPos">ターゲットの座標</param>
	/// <param name="TargetAngle">ターゲットの向き</param>
	/// <param name="TargetDis">ターゲットまでの距離</param>
	void setFollowTarget(XMFLOAT3* TargetPos, XMFLOAT3* TargetAngle, float TargetDis);

	/// <summary>
	/// 追従処理
	/// </summary>
	void Following();

	//ターゲットの座標
	XMFLOAT3* TargetObjectPos = nullptr;

	//ターゲットの向き
	XMFLOAT3* TargetObjectAngle = nullptr;

	//ターゲットとの距離
	float TargetDistance = 0.0f;
};
