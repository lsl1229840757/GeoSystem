#pragma once
#include <vector>
#include "Geometry.h"
#include <QJsonObject>
#include <SymbolStyle.h>
#include "ogrsf_frmts.h"
#include "qmap.h"
#include "qvariant.h"
using namespace std;
class Feature
{
public:
	Feature(void);
	~Feature(void);
	 //��������
    Geometry* geometry;
	int featureID;
	OGRGeometry *ogrGrom;  //OGR���ζ��󣬱����жϽ���
    QJsonObject properties;  //json��properties����
	SymbolStyle symbolStyle;
	QVariantMap attributes;  //ӳ���¼�����ֶε������������ԣ�QMap����map���ܴ�ű��������//QMap<QString, QVariant>
	bool isSelected; //�Ƿ񱻲�ѯ��ѡ��
};
