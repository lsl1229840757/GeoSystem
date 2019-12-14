#pragma once
#include <QJsonArray>
typedef enum{
	GEOPOINT,GEOPOLYLINE,GEOPOLYGON,GEOMULTIPOLYGON,GEOMULTIPOLYLINE
} GeometryType;
//设定命名空间，防止与GEOS库的Geometry命名冲突
namespace mgeo
{
	class Geometry
	{
	public:
		Geometry(void);
		~Geometry(void);

		//定义一些判断类型的常量
		QJsonArray coordinates;
		GeometryType virtual getGeometryType() = 0;
	};
}
