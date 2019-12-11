#include "MapPrjMercator.h"



MapPrjMercator::MapPrjMercator()
{
}


MapPrjMercator::~MapPrjMercator()
{
}


MapPrjType MapPrjMercator::getMapPrjType()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	return MapPrjType::MERCATOR;
}


void MapPrjMercator::getXY(double x1,double y1,double *px,double *py)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	//int a = 6378137;
	double xx = x1 * 1.0 * a*Pi / 180;
	double d = (90 + y1 * 1.0)*(Pi / 360);
	double c = tan((90 + y1 * 1.0)*(Pi / 360));
	double k = log(tan((90 + y1 * 1.0)*(Pi / 360)));
	double yy = log(tan((90 + y1 * 1.0 )*(Pi / 360)));
	yy = yy * a;
	*px = xx;
	*py = yy;
}



