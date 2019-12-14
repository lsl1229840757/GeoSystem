#pragma once
#include <vector>
#include "MGeometry.h"
#include <QJsonObject>
#include <SymbolStyle.h>
#include "ogrsf_frmts.h"
#include "qmap.h"
#include "qvariant.h"
#include "geos.h"
using namespace std;
class Feature
{
public:
	Feature(void);
	~Feature(void);
	 //��������
    mgeo::Geometry* geometry;
	int featureID;
	OGRGeometry *ogrGeom;  //OGR���ζ��󣬱����жϽ��� ,Json��ȡʱ���洢OGR����
	geos::geom::Geometry* geosGeom;  //��GEOS�жϽ������洢�����ӿ��ٶ�
    QJsonObject properties;  //json��properties����
	SymbolStyle symbolStyle;
	QVariantMap attributes;  //ӳ���¼�����ֶε������������ԣ�QMap����map���ܴ�ű��������//QMap<QString, QVariant>
	bool isSelected; //�Ƿ񱻲�ѯ��ѡ��
	bool isFirstProjeted;  //feature �Ƿ��һ��ͶӰ
};
