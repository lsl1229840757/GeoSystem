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

	//����һЩ�ж����͵ĳ���
    QJsonArray coordinates;
	GeometryType virtual getGeometryType() = 0;
};

