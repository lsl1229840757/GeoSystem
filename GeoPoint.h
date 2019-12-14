#pragma once
#include "Geometry.h"
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
};