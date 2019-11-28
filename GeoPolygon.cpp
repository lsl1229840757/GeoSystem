#include "GeoPolygon.h"


GeoPolygon::GeoPolygon(void)
{
}


GeoPolygon::~GeoPolygon(void)
{
}

GeometryType GeoPolygon::getGeometryType(){
	return GeometryType::GEOPOLYGON;
}