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
	// TODO: �ڴ˴����ʵ�ִ���.
	//wkt�ķ�ʽתΪGEOS����Ҳ����wkb ���������±���Gdal
	if (ogrGeom == NULL || ogrGridBound == NULL)
	{
		return false;
	}
	int featureType = ogrGeom->getGeometryType();
	OGRGeometry *intersection = ogrGeom->Intersection(ogrGridBound);  //�ж��Ƿ��ཻ
	if (intersection != NULL)
	{
		return true;
	}
	else {
		return false;
	}
}
