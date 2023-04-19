#include"FollowCamera.h"
#include"../Base/WindowGenerate.h"

FollowCamera::FollowCamera()
{
	aspectRatio = (float)windowWidth / windowHight;

	//ビュー行列の計算
	UpdateViewMatrix();

	// 射影行列の計算
	UpdateProjectionMatrix();

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}

void FollowCamera::setFollowTarget(XMFLOAT3 TargetPos,XMFLOAT3 TargetAngle, const float TargetDis)
{
	//ターゲット対象をセット
	TargetObjectPos = &TargetPos;
	TargetObjectAngle = &TargetAngle;
	TargetDistance = TargetDis;
	target = *TargetObjectPos;
	eye = { target.x,target.y,target.z + TargetDistance };
	up = { 0,1,0 };
}

void FollowCamera::Following(XMFLOAT3 upVector, XMFLOAT3 forwordVector, XMFLOAT3 targetPos)
{
	//追従するターゲットの座標をセット
	target = targetPos;
	XMVECTOR targetPosition = XMLoadFloat3(&target);

	//クォータニオンのroll軸を反対向きにしたベクトルを作る
	XMFLOAT3 backVector = { -forwordVector.x,-forwordVector.y ,-forwordVector.z };

	//上のベクトルを正規化＆一定距離離す
	float backLength = sqrtf(powf(backVector.x, 2) + powf(backVector.y, 2) + powf(backVector.z, 2));
	backLength = 1 / backLength;
	backVector = {
		backVector.x * backLength * TargetDistance,
		backVector.y * backLength * TargetDistance,
		backVector.z * backLength * TargetDistance
	};

	//ターゲットの座標から上のベクトル引いた位置にカメラをセット(三人称視点)
	XMVECTOR eyePosition = targetPosition + XMLoadFloat3(&backVector);

	//ビュー行列生成
	matView = XMMatrixLookAtLH(eyePosition, targetPosition, XMLoadFloat3(&upVector));

	//プロジェクション行列生成
	UpdateProjectionMatrix();

	//ビュー行列×プロジェクション行列
	matViewProjection = matView * matProjection;

	//基底クラスのtarget・eye・upに計算した結果をセット
	XMStoreFloat3(&target, targetPosition);
	XMStoreFloat3(&eye, eyePosition);
	XMStoreFloat3(&up, XMLoadFloat3(&upVector));

	//基底カメラクラスの更新処理
	Update();

	//向いている方向に進む正規化ベクトルを生成
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