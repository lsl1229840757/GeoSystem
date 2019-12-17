#include "EuclideanDistanceUtil.h"



EuclideanDistanceUtil::EuclideanDistanceUtil()
{
}


EuclideanDistanceUtil::~EuclideanDistanceUtil()
{
}

double EuclideanDistanceUtil::computeDistance(double x1, double y1, double x2, double y2)
{
	//º∆À„≈∑ Ωæ‡¿Î
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
