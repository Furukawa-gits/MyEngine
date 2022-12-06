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
	void setFollowTarget(XMFLOAT3 TargetPos, XMFLOAT3 TargetAngle, const float TargetDis);

	/// <summary>
	/// 追従処理
	/// カメラの座標軸：クォータニオンの回転軸をセット
	/// カメラの座標：前方向の回転軸から逆算
	/// </summary>
	/// <param name="upVector">上方向の回転軸</param>
	/// <param name="forwordVector">前方向の回転軸</param>
	/// <param name="targetPos">追従する対象の座標</param>
	void Following(XMFLOAT3 upVector, XMFLOAT3 forwordVector, XMFLOAT3 targetPos);

	/// <summary>
	/// 正面に進む
	/// </summary>
	/// <param name="speed">移動速度</param>
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

	//ターゲットの座標
	XMFLOAT3* TargetObjectPos = nullptr;

	//ターゲットの向き
	XMFLOAT3* TargetObjectAngle = nullptr;

	//ターゲットとの距離
	float TargetDistance = 0.0f;

	//進行方向ベクトル
	XMFLOAT3 frontVec = { 0,0,0 };
};
