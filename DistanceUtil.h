#pragma once
typedef enum {
	EuclideanDistance
} DistanceType;
class DistanceUtil
{
public:
	DistanceUtil();
	~DistanceUtil();
	virtual double computeDistance(double x1, double y1, double x2, double y2) = 0;
};