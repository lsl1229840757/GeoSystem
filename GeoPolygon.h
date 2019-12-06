#pragma once
#include "Geometry.h"
#include "GeoPoint.h"
#include <vector>
#include "triangulate.h"
using namespace std;
class GeoPolygon:public Geometry
{
public:
	GeoPolygon(void);
	~GeoPolygon(void);
	vector<GeoPoint*> points;
	GeometryType getGeometryType();
	//判断凸凹性
	double multiply(GeoPoint* op, GeoPoint* p1, const GeoPoint* p2);
	bool isConvex();
	//得到三角剖分的polygon
	vector<GeoPolygon*> getTriangles();
};

