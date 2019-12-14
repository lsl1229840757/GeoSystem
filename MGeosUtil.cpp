#include "MGeosUtil.h"



MGeosUtil::MGeosUtil()
{
}


MGeosUtil::~MGeosUtil()
{
}


//OGRGeometry to GEOS Geometry
//防止循环引用，所以写在MGEOSUtil中
geos::geom::Geometry* MGeosUtil::OGR2GEOSGeom(OGRGeometry *ogrGeom)
{
	// TODO: 在此处添加实现代码.
	geos::io::WKTReader wktReader;
	char* buff = NULL;
	ogrGeom->exportToWkt(&buff);  //转为wkt,提高效率可用wkb
	string buffstr = buff;
	geos::geom::Geometry *geosGeometry = wktReader.read(buffstr);
	return geosGeometry;
}

GeometryFactory* MGeosUtil::getPGeometryFactory()
{
	// TODO: 在此处添加实现代码.
	//获取factory指针
	return GeometryFactory::create().get();
}
