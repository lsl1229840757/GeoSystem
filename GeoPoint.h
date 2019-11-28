#pragma once
#include "Geometry.h"
class GeoPoint:public Geometry
{
public:
	GeoPoint(void);
	~GeoPoint(void);
	double x;
	double y;
	GeometryType getGeometryType();
};