#include "SpatialIndex.h"


SpatialIndex::SpatialIndex()
{
	isIndexCreated = false;
}




SpatialIndex::~SpatialIndex()
{
}


bool SpatialIndex::isIntersect(OGRGeometry *ogrGeom, OGRGeometry *ogrGridBound)
{
	// TODO: 在此处添加实现代码.
	//wkt的方式转为GEOS对象，也可用wkb 。避免重新编译Gdal
	if (ogrGeom == NULL || ogrGridBound == NULL)
	{
		return false;
	}
	int featureType = ogrGeom->getGeometryType();
	OGRGeometry *intersection = ogrGeom->Intersection(ogrGridBound);  //判断是否相交
	if (intersection != NULL)
	{
		return true;
	}
	else {
		return false;
	}
}
