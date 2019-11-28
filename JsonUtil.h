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
	 //�����쳣
    QJsonObject static JsonRead(QString filePath) throw(runtime_error);
    //����key�еĳ������жϵ�һ��ṹkey
    static QString TYPE;
    static QString GEOMETRY;
    static QString PROPERTIES;
    //�����ж��������
    static QString FEATURE;
    static QString POINT;
    static QString POLYLINE;
    static QString POLYGON;
    //����key
    static QString COORDINATES;

    Feature static parseGeoJson(QJsonObject geoJson);
    Feature static parseFeature(QJsonObject featureJson) throw(runtime_error);
};

