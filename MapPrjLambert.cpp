#include "MapPrjLambert.h"



MapPrjLambert::MapPrjLambert()
{
}


MapPrjLambert::~MapPrjLambert()
{
}


// 获取投影类型
MapPrjType MapPrjLambert::getMapPrjType()
{
	// TODO: 在此处添加实现代码.
	return MapPrjType::LAMBERT;
}


void MapPrjLambert::getXY(double x1, double y1, double *px, double *py)
{
	// TODO: 在此处添加实现代码.
	int B1 = 27, B2 = 45;
	double rb1 = a*cos(B1*Pi/180.0) / pow((1 - pow(e, 2)*pow(sin(B1*Pi / 180.0), 2)), 0.5);
	double rb2 = a * cos(B2*Pi / 180.0) / pow((1 - pow(e, 2)*pow(sin(B2*Pi / 180.0), 2)), 0.5);
	//double ac = 0.590276175, C = 12840667;   //当割投影纬度为27N和45N
	double U = tan((90 + y1 * 1.0 )*(Pi / 360))*pow(((1 - e * sin((y1 * 1.0 )*(Pi / 180))) / (1 + e * sin(y1 * 1.0 )*(Pi / 180))), e / 2);
	double Ub1 = tan((90 + B1 * 1.0)*(Pi / 360))*pow(((1 - e * sin(B1*1.0*(Pi / 180))) / (1 + e * sin(B1*1.0*(Pi / 180)))), e / 2);
	double Ub2 = tan((90 + B2 * 1.0)*(Pi / 360))*pow(((1 - e * sin(B2*1.0*(Pi / 180))) / (1 + e * sin(B2*1.0*(Pi / 180)))), e / 2);
	double ac = (log(rb2) - log(rb1)) / (log(Ub1) - log(Ub2));
	double C = rb1 * pow(Ub1, ac) / ac; 
	int L0 = 105;
	double Rho = C / pow(U, ac), Rhos = C / pow(Ub1, ac);
	float delta = (ac * x1*1.0 ) * (Pi / 180); //
	float xx, yy;
	xx = Rhos - Rho * cos(delta);
	yy = Rho * sin(delta);
	*px = xx;
	*py = yy;
}





void MapPrjLambert::getBL(double x1, double y1, double *pL, double *pB)
{
	// TODO: 在此处添加实现代码.
}
