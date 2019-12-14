#include "MGeosUtil.h"



MGeosUtil::MGeosUtil()
{
}


MGeosUtil::~MGeosUtil()
{
}


//OGRGeometry to GEOS Geometry
//��ֹѭ�����ã�����д��MGEOSUtil��
geos::geom::Geometry* MGeosUtil::OGR2GEOSGeom(OGRGeometry *ogrGeom)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	geos::io::WKTReader wktReader;
	char* buff = NULL;
	ogrGeom->exportToWkt(&buff);  //תΪwkt,���Ч�ʿ���wkb
	string buffstr = buff;
	geos::geom::Geometry *geosGeometry = wktReader.read(buffstr);
	return geosGeometry;
}

GeometryFactory* MGeosUtil::getPGeometryFactory()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	//��ȡfactoryָ��
	return GeometryFactory::create().get();
}
