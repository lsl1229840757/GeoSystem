#pragma once
#include "qrect.h"
typedef enum {
	LAMBERT,MERCATOR
} MapPrjType;
class MapPrjUtil
{
public:
	MapPrjUtil();
	~MapPrjUtil();
	const double a = 6378245, b = 6356863;	//�����ᣬ�̰���
	double e;  //��һƫ����
	virtual void getXY(double x1, double y1, double *px, double *py) =0;  //����ͶӰ���XY
	double prjx, prjy;
	QRectF prjMapRange; 
	MapPrjType virtual getMapPrjType() = 0;
	const double Pi = 3.14159265;
	const double E = 2.718281828;   //��Ȼ��������
	// ��ȡͶӰ��ľ�������
	virtual QRectF getPrjRange(QRectF dataRange);
	bool mapPrjChanged;
	
	virtual void getBL(double x1, double y1, double *pL, double *pB) = 0;
	virtual QRectF getPrimRange(QRectF prjRange);
};

