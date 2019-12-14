#include "SpatialIndex.h"


SpatialIndex::SpatialIndex()
{
	isIndexCreated = false;
}




SpatialIndex::~SpatialIndex()
{
}

//判断相交,通过转换为GEOS对象，效率过低
bool SpatialIndex::isIntersect(OGRGeometry *ogrGeom, OGRGeometry *ogrGridBound)
{
	// TODO: 在此处添加实现代码.
	if (ogrGeom == NULL || ogrGridBound == NULL)
	{
		return false;
	}
	int featureType = ogrGeom->getGeometryType();
	//wkt的方式转为GEOS对象，也可用wkb 。避免重新编译Gdal
	geos::geom::Geometry* geosGeom = MGeosUtil::OGR2GEOSGeom(ogrGeom);
	geos::geom::Geometry* geosGridBound = MGeosUtil::OGR2GEOSGeom(ogrGridBound);
	//geos::geom::Geometry* intersection = geosGeom->intersection(geosGridBound); //判断是否相交
	if (geosGeom->disjoint(geosGridBound))
	{  //判断不相交
		return false;
	}
	else {
		return true;
	}
}

//GEOS Geometry 判断是否相交
bool SpatialIndex::isIntersect(geos::geom::Geometry *geosGeom, geos::geom::Geometry *geosGridBound)
{
	if (geosGeom == NULL || geosGridBound == NULL)
	{
		return false;
	}
	//geos::geom::Geometry* intersection = geosGeom->intersection(geosGridBound); //判断是否相交
	
	//尝试GEOS C API
	//GEOSContextHandle_t contexHandle = GEOS_init_r();
	//qDebug() << GEOSversion();
	//GEOS_finish_r(contexHandle);
	//qDebug() <<"geosGeom:"<<geosGeom->getSRID();
	//qDebug() << "geosBound:" << geosGridBound->getSRID();
	//geos::geom::Geometry *intersection = geosGeom->intersection(geosGridBound);
	//只判断是否有交，提高效率
	if (geosGeom->disjoint(geosGridBound))
	{  //判断不相交
		return false;
	}
	else {
		return true;
	}
}


