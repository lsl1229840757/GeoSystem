#pragma once
#include "Geometry.h"
#include "GeoPoint.h"
#include <vector>
#include "gpc.h"
using namespace std;
class GeoPolygon:public mgeo::Geometry
{
public:
	GeoPolygon(void);
	~GeoPolygon(void);
	vector<GeoPoint*> points;
	GeometryType getGeometryType();
	//�ж�͹����
	double multiply(GeoPoint* op, GeoPoint* p1, const GeoPoint* p2);
	bool isConvex();
	//�õ������ʷֵ�polygon
	vector<GeoPolygon*> getTriangles();
	vector<GeoPolygon*> triangles;
};

