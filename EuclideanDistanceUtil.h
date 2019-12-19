#pragma once
#include "DistanceUtil.h"
#define _USE_MATH_DEFINES 
#include <math.h>
class EuclideanDistanceUtil :
	public DistanceUtil
{
public:
	EuclideanDistanceUtil();
	~EuclideanDistanceUtil();
	double computeDistance(double x1, double y1, double x2, double y2);
	double computeDistanceByLatLng(double lat1, double lng1, double lat2, double lng2);
};