#include "GeoPoint.h"


GeoPoint::GeoPoint(void)
{
}

GeoPoint::GeoPoint(double x, double y)
{
	this->x = x;
	this->y = y;
}


GeoPoint::~GeoPoint(void)
{
}

GeometryType GeoPoint::getGeometryType(){

	return GeometryType::GEOPOINT;
}