#include"SplineCurve.h"

SplineCurve::SplineCurve()
{
}

SplineCurve::~SplineCurve()
{
}

void SplineCurve::setSpline(RKDVector3* anyposition, int allpointnum, int totalframe)
{
	//§Œä“_‚ª‘«‚è‚È‚¢ê‡
	if (allpointnum < 4) { return; }

	for (int i = 0; i < allpointnum; i++) {
		controlPoints.push_back(anyposition[i]);
	}

	this->nowPlayIndex = 1;

	this->totalFrame = totalframe;
	this->nowFrame = 0;

	this->oneCurveFrame = totalFrame / (controlPoints.size() - 3);

	this->startPosition = controlPoints[0];
}

void SplineCurve::play()
{
	this->isPlay = true;
}

RKDVector3 SplineCurve::updata()
{
	if (!isPlay) { return startPosition; }
	if (nowFrame >= totalFrame) { return returnPosition; }

	size_t n = controlPoints.size() - 2;

	if (nowPlayIndex > n) return controlPoints[n];
	if (nowPlayIndex < 1) return controlPoints[1];

	nowFrame++;

	nowOneCurveRate = nowFrame / oneCurveFrame;

	nowOneCurveRate = std::min(nowOneCurveRate, 1.0f);

	if (nowOneCurveRate >= 1.0f)
	{
		if (nowPlayIndex < controlPoints.size() - 3)
		{
			nowPlayIndex++;
			nowOneCurveRate -= 1.0f;
			nowFrame = 0;
		}
		else
		{
			nowOneCurveRate = 1.0f;
		}
	}

	RKDVector3& p0 = controlPoints[nowPlayIndex - 1];
	RKDVector3& p1 = controlPoints[nowPlayIndex];
	RKDVector3& p2 = controlPoints[nowPlayIndex + 1];
	RKDVector3& p3 = controlPoints[nowPlayIndex + 2];

	returnPosition = splineCurve4(p0, p1, p2, p3, nowOneCurveRate);

	return returnPosition;
}

void SplineCurve::reset()
{
	isPlay = false;

	returnPosition = startPosition;
}

RKDVector3 SplineCurve::getCurveSubdivisionPoint(float now)
{
	return RKDVector3();
}

RKDVector3 SplineCurve::splineCurve4(const RKDVector3& p0, const RKDVector3& p1, const RKDVector3& p2, const RKDVector3& p3, float& t)
{
	RKDVector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t +
		(2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t +
		(-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);

	return position;
}
