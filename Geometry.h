#pragma once
#include <QJsonArray>
typedef enum{
	GEOPOINT,GEOPOLYLINE,GEOPOLYGON,GEOMETRY
} GeometryType;
class Geometry
{
public:
	Geometry(void);
	~Geometry(void);

	//定义一些判断类型的常量
    QJsonArray coordinates;
	GeometryType virtual getGeometryType() = 0;
};

