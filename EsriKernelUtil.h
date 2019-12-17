#pragma once
#include "KernelUtil.h"
class EsriKernelUtil :
	public KernelUtil
{
public:
	EsriKernelUtil();
	~EsriKernelUtil();
	vector<vector<double>> computeKernelUsingPoint(QRectF extent, vector<GeoPoint*> points, vector<double> population, double cellSize, double searchRadius, DistanceUtil* method);//使用点要素计算核
};

