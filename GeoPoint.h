#pragma once
#include "MGeometry.h"
#include <utility>
#include "MapPrjUtil.h"
using namespace std;
class GeoPoint:public mgeo::Geometry
{
public:
	GeoPoint(void);
	GeoPoint(double x, double y);
	~GeoPoint(void);
	double x;
	double y;
	GeometryType getGeometryType();
	double prjx, prjy;
	double isFirstProjeted;
	// //获取点坐标
	pair<double,double> getPtCoor(MapPrjUtil* mapPrj, bool isLastPt, bool &isFeaFirstPrj);
	double getX();
	double getY();
};