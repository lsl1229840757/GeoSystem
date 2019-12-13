#include "GeoPoint.h"


GeoPoint::GeoPoint(void)
{
	isFirstProjeted = true;
}

GeoPoint::GeoPoint(double x, double y)
{
	isFirstProjeted = true;
	this->x = x;
	this->y = y;
}


GeoPoint::~GeoPoint(void)
{
}

GeometryType GeoPoint::getGeometryType(){

	return GeometryType::GEOPOINT;
}