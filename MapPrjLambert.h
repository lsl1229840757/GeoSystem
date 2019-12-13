#pragma once
#include "MapPrjUtil.h"
class MapPrjLambert :
	public MapPrjUtil
{
public:
	MapPrjLambert();
	virtual~MapPrjLambert();
	// 获取投影类型
	MapPrjType getMapPrjType();
	
	virtual void getXY(double x1, double y1, double *px, double *py);

	virtual void getBL(double x1, double y1, double *pL, double *pB);
};

