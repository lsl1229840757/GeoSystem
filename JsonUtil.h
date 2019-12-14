#pragma once
#include <QJsonObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <exception>
#include "Feature.h"
#include "Geometry.h"
#include "GeoPoint.h"
#include "GeoPolyline.h"
#include "GeoPolygon.h"
#include "qmessagebox.h"
#include "GeoMap.h"
#include "Layer.h"
#include "GeoMultiPolygon.h"
#include "geos.h"
#include "MGeosUtil.h"
class JsonUtil
{
public:
	JsonUtil(void);
	~JsonUtil(void);
	 //存在异常
    QJsonObject static JsonRead(QString filePath) throw(runtime_error);
    //定义key中的常量，判断第一层结构
    static QString TYPE;
	static QString FEATURECOLLECTION;
	static QString GEOMETRYCOLLECTION;
	static QString FEATURES;
	static QString GEOMETRYS;
	//第二层
	static QString GEOMETRY;
	static QString PROPERTIES;
    //常量判断相关类型
    static QString FEATURE;
    static QString POINT;
    static QString POLYLINE;
    static QString POLYGON;
	static QString MULTIPOLYGON;

    //辅助key
    static QString COORDINATES;

    static GeoMap* parseGeoJson(QString filePath);
private:
    static QRectF parseFeature(QJsonObject feaJObj,Layer* layer) throw(runtime_error);
	static void parseFeatureCollection(QJsonObject geoJson, GeoMap* geoMap);
	static void parseGeometryCollection(QJsonObject geoJson, GeoMap* geoMap);
	static QRectF parseGeometry(QJsonObject geomJObj, Feature* feature);
public:
	// 写入Json文件
	static void jsonWrite(QString destPath, QJsonObject* jsonObj);
	static QJsonObject storeDbParams(QString tableName, QString dbname, QString addr, QString port, QString username, QString password);
	static void loadDbParams(QJsonObject* pJsonObj, QString& tableName, QString& dbname, QString& addr, QString& port, QString& username, QString& password);
};

