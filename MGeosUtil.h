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
	static GeometryFactory geosGeomFactory;  //����ʵ����������GEOSͼ�ο��ɴ˶��󴴽�
};

