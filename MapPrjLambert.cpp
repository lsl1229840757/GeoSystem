#include "MapPrjLambert.h"



MapPrjLambert::MapPrjLambert()
{
}


MapPrjLambert::~MapPrjLambert()
{
}


// ��ȡͶӰ����
MapPrjType MapPrjLambert::getMapPrjType()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	return MapPrjType::LAMBERT;
}


void MapPrjLambert::getXY(double x1, double y1, double *px, double *py)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	int B1 = 27, B2 = 45;
	double ac = 0.590276175, C = 12840667;   //����ͶӰγ��Ϊ27N��45N
	double U = tan((90 + y1 * 1.0 )*(Pi / 360))*pow(((1 - e * sin((y1 * 1.0 )*(Pi / 180))) / (1 + e * sin(y1 * 1.0 )*(Pi / 180))), e / 2);
	double Ub1 = tan((90 + B1 * 1.0)*(Pi / 360))*pow(((1 - e * sin(B1*1.0*(Pi / 180))) / (1 + e * sin(B1*1.0*(Pi / 180)))), e / 2);
	int L0 = 120;
	double Rho = C / pow(U, ac), Rhos = C / pow(Ub1, ac);
	float delta = (ac * x1*1.0 ) * (Pi / 180);
	float xx, yy;
	xx = Rhos - Rho * cos(delta);
	yy = Rho * sin(delta);
	*px = xx;
	*py = yy;
}



