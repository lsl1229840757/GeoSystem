#pragma once
#include "Geometry.h"
#include "GeoPoint.h"
#include <vector>
using namespace std;
class GeoPolyline:public mgeo::Geometry
{
public:
	GeoPolyline(void);
	~GeoPolyline(void);
	vector<GeoPoint*> points;
	GeometryType getGeometryType();
};


