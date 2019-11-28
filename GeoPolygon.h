#pragma once
#include "Geometry.h"
#include "GeoPoint.h"
#include <vector>
using namespace std;
class GeoPolygon:public Geometry
{
public:
	GeoPolygon(void);
	~GeoPolygon(void);
	vector<GeoPoint*> points;
	GeometryType getGeometryType();
};

