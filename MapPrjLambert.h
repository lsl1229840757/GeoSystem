#pragma once
#include "MapPrjUtil.h"
class MapPrjLambert :
	public MapPrjUtil
{
public:
	MapPrjLambert();
	virtual~MapPrjLambert();
	// ��ȡͶӰ����
	MapPrjType getMapPrjType();
	
	virtual void getXY(double x1, double y1, double *px, double *py);

};

