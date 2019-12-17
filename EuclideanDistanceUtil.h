#pragma once
#include "DistanceUtil.h"
#include <math.h>
class EuclideanDistanceUtil :
	public DistanceUtil
{
public:
	EuclideanDistanceUtil();
	~EuclideanDistanceUtil();
	double computeDistance(double x1, double y1, double x2, double y2);
};

