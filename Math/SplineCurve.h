#pragma once
#include"RKDVector.h"
#include<vector>
#include <array>

class SplineCurve
{
public:
	SplineCurve();
	~SplineCurve();

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="anyposition">制御点(始点と終点を加えたもの)</param>
	/// <param name="allpointnum">制御点の数</param>
	/// <param name="totalframe">総フレーム</param>
	void setSpline(RKDVector3* anyposition, int allpointnum, int totalframe);

	void play();

	RKDVector3 updata();

	void reset();

	//補間中の一点を取得
	RKDVector3 getCurveSubdivisionPoint(float now);

private:

	//制御点
	std::vector<RKDVector3> controlPoints;

	//現在位置
	RKDVector3 returnPosition;

	//初期位置
	RKDVector3 startPosition;

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
	float nowTotalRate = 0;

	//１曲線あたりのフレームに対する現在フレームの割合
	float nowOneCurveRate = 0;

	//スプラインカーブ計算
	RKDVector3 splineCurve4(
		const RKDVector3& p0, 
		const RKDVector3& p1, 
		const RKDVector3& p2, 
		const RKDVector3& p3, float& t);
};