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
	//êßå‰ì_
	std::vector<XMFLOAT3> controlPoints;
	int nowFrame = 0;
	int totalFrame = 0;
	bool isPlay = false;
	XMFLOAT3 returnPosition;
	int nowPlayIndex = 0;

	XMFLOAT3 splineCurve4(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3);
};