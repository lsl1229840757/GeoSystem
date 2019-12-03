#pragma once
#include "Geometry.h"
#include <vector>
#include "GeoPolyline.h"
using namespace std;

class GeoMultiPolyline :
	public Geometry
{
public:
	GeoMultiPolyline();
	virtual ~GeoMultiPolyline();
	vector<GeoPolyline*> polylines;
	GeometryType getGeometryType();
};

