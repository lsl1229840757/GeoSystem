#include "GeoPoint.h"


GeoPoint::GeoPoint(void)
{
}


GeoPoint::~GeoPoint(void)
{
}

GeometryType GeoPoint::getGeometryType(){

	return GeometryType::GEOPOINT;
}