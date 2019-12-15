#include "SpatialIndex.h"


SpatialIndex::SpatialIndex()
{
	isIndexCreated = false;
}




SpatialIndex::~SpatialIndex()
{
}

//�ж��ཻ,ͨ��ת��ΪGEOS����Ч�ʹ���
bool SpatialIndex::isIntersect(OGRGeometry *ogrGeom, OGRGeometry *ogrGridBound)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (ogrGeom == NULL || ogrGridBound == NULL)
	{
		return false;
	}
	int featureType = ogrGeom->getGeometryType();
	//wkt�ķ�ʽתΪGEOS����Ҳ����wkb ���������±���Gdal
	geos::geom::Geometry* geosGeom = MGeosUtil::OGR2GEOSGeom(ogrGeom);
	geos::geom::Geometry* geosGridBound = MGeosUtil::OGR2GEOSGeom(ogrGridBound);
	//geos::geom::Geometry* intersection = geosGeom->intersection(geosGridBound); //�ж��Ƿ��ཻ
	if (geosGeom->disjoint(geosGridBound))
	{  //�жϲ��ཻ
		return false;
	}
	else {
		return true;
	}
}

//GEOS Geometry �ж��Ƿ��ཻ
bool SpatialIndex::isIntersect(geos::geom::Geometry *geosGeom, geos::geom::Geometry *geosGridBound)
{
	if (geosGeom == NULL || geosGridBound == NULL)
	{
		return false;
	}
	//geos::geom::Geometry* intersection = geosGeom->intersection(geosGridBound); //�ж��Ƿ��ཻ
	
	//����GEOS C API
	//GEOSContextHandle_t contexHandle = GEOS_init_r();
	//qDebug() << GEOSversion();
	//GEOS_finish_r(contexHandle);
	//qDebug() <<"geosGeom:"<<geosGeom->getSRID();
	//qDebug() << "geosBound:" << geosGridBound->getSRID();
	//geos::geom::Geometry *intersection = geosGeom->intersection(geosGridBound);
	//ֻ�ж��Ƿ��н������Ч��
	if (geosGeom->disjoint(geosGridBound))
	{  //�жϲ��ཻ
		return false;
	}
	else {
		return true;
	}
}


