#pragma once
#include"../FbxLoder/Quaternion.h"
#include<vector>

class SplineCurve
{
public:
	SplineCurve();
	~SplineCurve();

	void setSpline(XMFLOAT3* anyposition, int allpointnum, int totalframe);

	void play();

	XMFLOAT3 updata();

	void reset();

	std::vector<XMFLOAT3> getCurveSubdivisionPoint();

private:

	//制御点
	std::vector<XMFLOAT3> controlPoints;

	//現在位置
	XMFLOAT3 returnPosition;

	//現在のメイン制御点
	int nowPlayIndex = 0;

	//再生フラグ
	bool isPlay = false;

	//現在フレーム・総フレーム
	int nowFrame = 0;
	int totalFrame = 0;

	//１曲線当たりのフレーム(総フレームと制御点の都合上割り切れない可能性があるのでfloat型)
	float oneCurveFrame = 0;

	//総フレームに対する現在フレームの割合
	float nowFramePerTotalFrame = 0;

	//１曲線あたりのフレームに対する現在フレームの割合
	float nowFramePerOneCurveFrame = 0;

	//スプラインカーブ計算
	XMFLOAT3 splineCurve4(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3);
};