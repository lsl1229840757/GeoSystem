#include "EuclideanDistanceUtil.h"
#define EARTH_RADIUS            6378.137 


EuclideanDistanceUtil::EuclideanDistanceUtil()
{
}


EuclideanDistanceUtil::~EuclideanDistanceUtil()
{
}

double EuclideanDistanceUtil::computeDistance(double x1, double y1, double x2, double y2)
{
	//计算欧式距离
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double EuclideanDistanceUtil::computeDistanceByLatLng(double lat1, double lng1, double lat2, double lng2)
{
	double radLat1 = lat1 * M_PI / 180.0;   //角度1˚ = π / 180
	double radLat2 = lat2 * M_PI / 180.0;   //角度1˚ = π / 180
	double a = fabs(radLat1 - radLat2);//纬度之差
	double b = fabs(lng1 * M_PI / 180.0 - lng2 * M_PI / 180.0);  //经度之差
	double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2))));
	dst = dst * EARTH_RADIUS;
	dst = round(dst * 1000);   //原本是10000,结果大了10倍
	return dst;
}
