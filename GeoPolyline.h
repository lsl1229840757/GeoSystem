#pragma once
#include "Geometry.h"
#include "GeoPoint.h"
#include <vector>
using namespace std;
class GeoPolyline:public Geometry
{
public:
	GeoPolyline(void);
	~GeoPolyline(void);
	vector<GeoPoint*> points;
	GeometryType getGeometryType();
};


