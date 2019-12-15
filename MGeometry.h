#pragma once
#include <QJsonArray>
typedef enum{
	GEOPOINT,GEOPOLYLINE,GEOPOLYGON,GEOMULTIPOLYGON,GEOMULTIPOLYLINE
} GeometryType;
//�趨�����ռ䣬��ֹ��GEOS���Geometry������ͻ
namespace mgeo
{
	class Geometry
	{
	public:
		Geometry(void);
		~Geometry(void);

		//����һЩ�ж����͵ĳ���
		QJsonArray coordinates;
		GeometryType virtual getGeometryType() = 0;
	};
}
