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
	double x = x1 *20037508.34 / 180;
	double y = log(tan((90 + y1)*M_PI / 360)) / (M_PI / 180);
	y = y * 20037508.34 / 180;
	*px = x;
	*py = y;

	// TODO: �ڴ˴����ʵ�ִ���.
	//int a = 6378137;
	/*double xx = x1 * 1.0 * a*Pi / 180;
	double d = (90 + y1 * 1.0)*(Pi / 360);
	double U = tan((90 + y1 * 1.0)*(Pi / 360))*pow((1-e*sin(y1))/(1+e*sin(y1)),e/2);
	double k = log(U);
	double yy = k;
	yy = yy * a;
	*px = xx;
	*py = yy;*/
}





void MapPrjMercator::getBL(double x1, double y1, double *pL, double *pB)
{
	double x = x1 / 20037508.34 * 180;
	double y = y1 / 20037508.34 * 180;
	y = 180 / M_PI * (2 * atan(exp(y*M_PI / 180)) - M_PI / 2);
	*pL = x;
	*pB = y;

	// TODO: �ڴ˴����ʵ�ִ���.
	//int iterativeTimes = 10;     //��������Ϊ10
	//double iterativeValue = 0;        //������ʼֵ
	//double ll = (x1 / a)*(180 / Pi);
	//double bb = iterativeValue;
	//for (int i = 0; i < iterativeTimes; i++) {
	//	bb = Pi/2-(2 * atan(pow(e, (y1 / a))* pow(E, (e / 2)*log((1 - e * sin(bb)) / (1 + e * sin(bb))))));
	//}
	//bb = (bb-Pi/4)*(180 / Pi); //???��������45��?
	//*pL = ll;
	//*pB = bb;
}
