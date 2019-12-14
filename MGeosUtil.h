#pragma once
#include "geos.h"
#include "ogrsf_frmts.h"
using namespace std;
class MGeosUtil
{
public:
	MGeosUtil();
	~MGeosUtil();
	static geos::geom::Geometry* OGR2GEOSGeom(OGRGeometry *ogrGeom);
	GeometryFactory *geosGeomFactory;  //必须实例化，所有GEOS图形可由此对象创建。不能创建为静态
	static const GeometryFactory* getDefaultGeometryFactory();
};

