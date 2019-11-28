#include "GeoPolyline.h"


GeoPolyline::GeoPolyline(void)
{
}


GeoPolyline::~GeoPolyline(void)
{
}

GeometryType GeoPolyline::getGeometryType(){
	return GeometryType::GEOPOLYLINE;
}