#pragma once
#include <QJsonArray>
typedef enum{
	GEOPOINT,GEOPOLYLINE,GEOPOLYGON,MULTIPOLYGON
} GeometryType;
class Geometry
{
public:
	Geometry(void);
	~Geometry(void);

	//����һЩ�ж����͵ĳ���
    QJsonArray coordinates;
	GeometryType virtual getGeometryType() = 0;
};

