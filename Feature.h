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
	 //定义属性
    mgeo::Geometry* geometry;
	int featureID;
	OGRGeometry *ogrGeom;  //OGR几何对象，便于判断交集 ,Json读取时不存储OGR对象
	geos::geom::Geometry* geosGeom;  //用GEOS判断交集，存储起来加快速度
    QJsonObject properties;  //json的properties数据
	SymbolStyle symbolStyle;
	QVariantMap attributes;  //映射记录各个字段的属性名与属性，QMap避免map不能存放变体的问题//QMap<QString, QVariant>
	bool isSelected; //是否被查询后选中
	bool isFirstProjeted;  //feature 是否第一次投影
};
