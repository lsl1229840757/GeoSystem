#pragma once
#include <vector>
#include "GeoPoint.h"
#include "DistanceUtil.h"
#include <qrect.h>
#include <utility>
#define _USE_MATH_DEFINES 
#include <math.h>
using namespace std;
class KernelUtil
{
public:
	KernelUtil();
	~KernelUtil();
	//点要素范围, 点要素集合，population字段， cell大小， 搜索半径， 距离类型
	virtual vector<vector<double>> computeKernelUsingPoint(QRectF extent, vector<GeoPoint*> points, vector<double> population, double cellSize, double searchRadius, DistanceUtil* method) = 0;//使用点要素计算核
	//这里的bottom是逻辑上y的最小值
	static pair<double, double> getCdByNum(double left, double bottom, double cellSize, int wNum, int hNum);
};