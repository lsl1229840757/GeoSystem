#include "GeoMultiPolygon.h"



GeoMultiPolygon::GeoMultiPolygon()
{
}


GeoMultiPolygon::~GeoMultiPolygon()
{
}


GeometryType GeoMultiPolygon::getGeometryType()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	return GeometryType::MULTIPOLYGON;
}
