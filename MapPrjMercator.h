#pragma once
#include "MapPrjUtil.h"
class MapPrjMercator :
	public MapPrjUtil
{
public:
	MapPrjMercator();
	virtual ~MapPrjMercator();
	MapPrjType getMapPrjType();
	virtual void getXY(double x1,double y1, double *px, double *py);

	virtual void getBL(double x1, double y1, double *pL, double *pB);
};

