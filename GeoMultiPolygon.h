#pragma once
#include "Geometry.h"
#include <vector>
#include "GeoPolygon.h"

using namespace std;
class GeoMultiPolygon :
	public Geometry
{
public:
	GeoMultiPolygon();
	~GeoMultiPolygon();
	vector<GeoPolygon*> polygons;
	GeometryType getGeometryType();
};

