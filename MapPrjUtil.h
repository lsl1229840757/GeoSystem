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
	const double a = 6378245, b = 6356863;	//长半轴，短半轴
	double e;  //第一偏心率
	virtual void getXY(double x1, double y1, double *px, double *py) =0;  //返回投影后的XY
	double prjx, prjy;
	QRectF prjMapRange; 
	MapPrjType virtual getMapPrjType() = 0;
	const double Pi = 3.14159265;
	const double E = 2.718281828;   //自然对数底数
	// 获取投影后的矩形区域
	virtual QRectF getPrjRange(QRectF dataRange);
	bool mapPrjChanged;
	
	virtual void getBL(double x1, double y1, double *pL, double *pB) = 0;
	virtual QRectF getPrimRange(QRectF prjRange);
};

