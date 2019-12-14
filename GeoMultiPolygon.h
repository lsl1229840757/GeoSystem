#pragma once
#include "MGeometry.h"
#include <vector>
#include "GeoPolygon.h"

using namespace std;
class GeoMultiPolygon :
	public mgeo::Geometry
{
public:
	GeoMultiPolygon();
	~GeoMultiPolygon();
	vector<GeoPolygon*> polygons;
	GeometryType getGeometryType();
};

