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
class JsonUtil
{
public:
	JsonUtil(void);
	~JsonUtil(void);
	 //�����쳣
    QJsonObject static JsonRead(QString filePath) throw(runtime_error);
    //����key�еĳ������жϵ�һ��ṹ
    static QString TYPE;
	static QString FEATURECOLLECTION;
	static QString GEOMETRYCOLLECTION;
	static QString FEATURES;
	static QString GEOMETRYS;
	//�ڶ���
	static QString GEOMETRY;
	static QString PROPERTIES;
    //�����ж��������
    static QString FEATURE;
    static QString POINT;
    static QString POLYLINE;
    static QString POLYGON;
	static QString MULTIPOLYGON;

    //����key
    static QString COORDINATES;

    static GeoMap* parseGeoJson(QJsonObject geoJson);
private:
    static QRectF parseFeature(QJsonObject feaJObj,Layer* layer) throw(runtime_error);
	static void parseFeatureCollection(QJsonObject geoJson, GeoMap* geoMap);
	static void parseGeometryCollection(QJsonObject geoJson, GeoMap* geoMap);
	static QRectF parseGeometry(QJsonObject geomJObj, Feature* feature);
};

