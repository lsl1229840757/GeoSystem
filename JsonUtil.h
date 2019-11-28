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
class JsonUtil
{
public:
	JsonUtil(void);
	~JsonUtil(void);
	 //存在异常
    QJsonObject static JsonRead(QString filePath) throw(runtime_error);
    //定义key中的常量，判断第一层结构key
    static QString TYPE;
    static QString GEOMETRY;
    static QString PROPERTIES;
    //常量判断相关类型
    static QString FEATURE;
    static QString POINT;
    static QString POLYLINE;
    static QString POLYGON;
    //辅助key
    static QString COORDINATES;

    Feature static parseGeoJson(QJsonObject geoJson);
    Feature static parseFeature(QJsonObject featureJson) throw(runtime_error);
};

